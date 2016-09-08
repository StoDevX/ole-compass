import sys
from OpenGL.GL import glClear, GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, glColor3f, \
                      glClearColor, glMatrixMode, glLoadIdentity, glOrtho, \
                      GL_PROJECTION, glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, \
                      glEnable, GL_BLEND, glViewport
from OpenGL.GLUT import GLUT_RGBA, GLUT_DOUBLE, GLUT_DEPTH, glutSwapBuffers, \
                        glutInit, glutInitDisplayMode, glutCreateWindow, glutDisplayFunc, \
                        glutMainLoop, glutInitWindowSize, glutInitWindowPosition, \
                        glutSpecialFunc, glutMouseFunc, glutMotionFunc, \
                        glutPostRedisplay, glutReshapeFunc, \
                        GLUT_LEFT_BUTTON, GLUT_DOWN, \
                        GLUT_KEY_RIGHT, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_DOWN

from helpers import draw_box, draw_text
from neighbor import Neighbor
from map import Map
from path import Path
from button import Button
from copy import deepcopy

ENTIRE_MAP_PATH = '../data/entireMapCalc.txt'
NORMAL_BUTTONS_PATH = '../data/buttonList.txt'
SPECIAL_BUTTONS_PATH = '../data/specialButtons.txt'
MAP_TEXTURE_PATH = '../StoMap.pam'

mouse_is_dragging = False
selection_finished = False
redraw_window = False
path_declared = False

prefer_indoors = False
avoid_stairs = False
draw_invisibles = False
draw_nodes = True
draw_all = True
draw_grid = False

global_neighbor = Neighbor(1, 1)
global_map_a = Map()
global_map_b = Map()
global_map_c = Map()
global_path = Path()

WIDTH = 940  # width of the user window (640 + 80) 720 1080
HEIGHT = 640 # height of the user window (480 + 60) 540 720
programName = "OleCompass"
campusmap = 0 # texture IDs

shift_x = 0
shift_y = 0
pan_rate = 50

no_of_segments = 0

start = -1
finish = -1

# button info
normal_buttons = []
special_buttons = []
NodeList = []

popup_yes = Button(200, 300, 60, 30, -1, "Yes")
popup_no = Button(300, 300, 60, 30, -1, "No")

def load_buttons_from_file(filename):
  btnlist = []
  with open(filename, 'r') as infile:
    _ = infile.readline()
    for line in infile:
      x, y, w, h, id, name = line.split()
      btnlist.append(Button(x, y, w, h, id, name))
  return btnlist


def load_normal_buttons(filename):
  global normal_buttons
  normal_buttons = load_buttons_from_file(filename)

def load_special_buttons(filename):
  global special_buttons
  special_buttons = load_buttons_from_file(filename)


def int_to_building(x):
  global NodeList
  for btn in normal_buttons:
    if x == btn.id:
      return btn.name
  return NodeList[x].id


# DRAWING WINDOW

def drawWindow():
  # clear the buffer
  global selection_finished, prefer_indoors, avoid_stairs
  glClear(GL_COLOR_BUFFER_BIT)

  # draw stuff
  # The original resolution of the image is 1053 x 775. We can change the
  # effective "zoom" of the map by increasing or decreasing this resolution.
  # However, try to keep the ratio of width to height as close as possible to
  # maintain the aspect ratio of the map so it isn't stretched or anything.
  # texID,   x,y,    width, height
  # drawTexture(campusmap, -600, -500, 1895, 1395)
  # drawing path

  # drawing the calculated path (if none, this is empty.)

  global_path.draw()

  # drawing ALL connections.

  # global_map_c.draw_connections(ENTIRE_MAP_PATH)

  if draw_grid:
    global_map_c.draw_connections(ENTIRE_MAP_PATH)

  # drawing nodes
  if draw_all and draw_nodes:
    global_map_a.draw_nodes(draw_invisibles, start, finish)
    # global_map_c.draw_connections(ENTIRE_MAP_PATH)

  # drawing buttons
  if draw_all:
    for btn in normal_buttons:
      btn.draw()

  for btn in special_buttons:
    btn.draw(0, 0, 100)
    if btn.id == -1:
      btn.draw(100, 0, 0)
    if btn.id == -6:
      if prefer_indoors:
        btn.name = "Unweigh Indoors"
        btn.draw(0, 0, 100)
      else:
        btn.name = "Weigh Indoors"
        btn.draw(0, 0, 100)
    if btn.id == -8:
      if avoid_stairs:
        btn.name = "Include stairs"
        btn.draw(0, 0, 100)
      else:
        btn.name = "Avoid stairs"
        btn.draw(0, 0, 100)

  glColor3f(1., 1., 1.)
  draw_text(10 - shift_x, 95 + shift_y, "Panning Rate")
  # popup display
  if selection_finished and path_declared:
    glColor3f(.5, .5, .5); # white
    draw_box(100 - shift_x, 100 + shift_y, 400, 300)
    glColor3f(0., 0., 0.); # text is black
    draw_text(150 - shift_x, 200 + shift_y, "Do you want to find the shortest")
    draw_text(150 - shift_x, 220 + shift_y, "path between")
    glColor3f(1., 0., 0.)
    draw_text(150 - shift_x, 240 + shift_y, int_to_building(start))
    glColor3f(0., 0., 0.)
    draw_text(150 - shift_x, 260 + shift_y, "and")
    glColor3f(1., 0., 0.)
    draw_text(150 - shift_x, 280 + shift_y, int_to_building(finish))
    # popup_yes.change_position(-shift_x+pan_rate, shift_y+pan_rate)
    # popup_no.change_position(-shift_x+pan_rate, shift_y+pan_rate)
    popup_yes.draw()
    popup_no.draw()
    if popup_yes.is_pressed or popup_no.is_pressed:
      selection_finished = False

  glutSwapBuffers()


def print_path():
  if finish is not -1:
    print("Selection Finished!")
    selection_finished = True
    print( "Starting at {} ({}), ending at {} ({})".format(int_to_building(start), start, int_to_building(finish), finish))

def fill_path(x):
  global start, finish
  if start == -1:
    start = x
  elif finish == -1:
    finish = x

# the mouse function is called when a mouse button is pressed down or released
def mouse(mouse_button, state, x, y):
  global no_of_segments, redraw_window, path_declared, draw_invisibles, \
         draw_nodes, draw_all, draw_grid, shift_x, shift_y, \
         pan_rate, start, finish
  if GLUT_LEFT_BUTTON == mouse_button:
    if GLUT_DOWN == state:
      # This std::cout prints the current location of the mouse upon a click
      # print( x-shift_x , ", " , y+shift_y )
      # print( shift_x , ", " , shift_y )
      for btn in normal_buttons:
        btn.is_pressed = btn.on_button(x, y, shift_x, -shift_y)

      for node in global_map_a:
        node.is_pressed = node.on_node(x, y, shift_x, -shift_y)

      for btn in special_buttons:
        btn.is_pressed = btn.on_button(x, y, shift_x, -shift_y)
        if btn.is_pressed:
          for btn in normal_buttons:
            btn.is_pressed = btn.on_button(x, y, shift_x, -shift_y)
          for node in global_map_a:
            node.is_pressed = node.on_node(x, y, shift_x, -shift_y)

      popup_yes.is_pressed = popup_yes.on_button(x, y, shift_x, -shift_y)
      popup_no.is_pressed = popup_no.on_button(x, y, shift_x, -shift_y)

    else:
      for btn in normal_buttons:
        if btn.on_button(x, y, shift_x, -shift_y) and btn.is_pressed:
          print(btn.name, "selected")
          fill_path(btn.id)
          print_path()
          btn.is_pressed = False

      for i, node in enumerate(global_map_a):
        if node.on_node(x, y, shift_x, -shift_y) and node.is_pressed:
          print("Node", i, "selected")
          fill_path(i)
          print_path()
          node.is_pressed = False

      # RESET BUTTON
      if special_buttons[0].on_button(x, y, shift_x, -shift_y) and special_buttons[0].is_pressed:
        print( "GLOBAL RESET" )
        global_path.empty_path()
        special_buttons[0].is_pressed = False
        start = finish = -1
        prefer_indoors = False
        selection_finished = False
        path_declared = False
        avoid_stairs = False
        draw_invisibles = False
        draw_nodes = True
        draw_all = True
        draw_grid = False

      # PREFER INDOORS BUTTON
      if special_buttons[5].on_button(x, y, shift_x, -shift_y) and special_buttons[5].is_pressed:
        print( "No longer preferring indoors" if prefer_indoors else "Prefering indoors" )
        prefer_indoors = not prefer_indoors
        global_map_b.load_map_calc_weighted(ENTIRE_MAP_PATH, prefer_indoors, avoid_stairs, False, False)
        special_buttons[5].is_pressed = False

      # AVOID STAIRS BUTTON
      if special_buttons[7].on_button(x, y, shift_x, -shift_y) and special_buttons[7].is_pressed:
        print( "Avoiding Stairs" if avoid_stairs else "No longer avoiding stairs" )
        avoid_stairs = not avoid_stairs
        global_map_b.load_map_calc_weighted(ENTIRE_MAP_PATH, prefer_indoors, avoid_stairs, False, False)
        special_buttons[7].is_pressed = False

      # DRAW ALL BUTTON
      if special_buttons[6].on_button(x, y, shift_x, -shift_y) and special_buttons[6].is_pressed:
        draw_all = not draw_all
        special_buttons[6].is_pressed = False

      # DRAW HIDDEN BUTTON
      if special_buttons[8].on_button(x, y, shift_x, -shift_y) and special_buttons[8].is_pressed:
        draw_invisibles = not draw_invisibles
        special_buttons[8].is_pressed = False

      # DRAW NODES BUTTON
      if special_buttons[9].on_button(x, y, shift_x, -shift_y) and special_buttons[9].is_pressed:
        draw_invisibles = not draw_invisibles
        special_buttons[9].is_pressed = False

      # DRAW GRID BUTTON
      if special_buttons[10].on_button(x, y, shift_x, -shift_y) and special_buttons[10].is_pressed:
        draw_invisibles = not draw_invisibles
        special_buttons[10].is_pressed = False

      if special_buttons[11].on_button(x, y, shift_x, -shift_y) and special_buttons[11].is_pressed:
        pan_rate += 5
        special_buttons[11].is_pressed = False
        print( "Pan Rate changed to: " , pan_rate )

      if special_buttons[12].on_button(x, y, shift_x, -shift_y) and special_buttons[12].is_pressed:
        pan_rate -= 5
        special_buttons[12].is_pressed = False
        print( "Pan Rate changed to: " , pan_rate )

      # ON SCREEN PANNING BUTTONS
      for btn in special_buttons[:4]:
        if btn.on_button(x, y, shift_x, -shift_y) and btn.is_pressed:
          btn.is_pressed = False
          if btn.id == -4:
            if shift_x + pan_rate < 500:
              glTranslatef(pan_rate, 0., 0.)
              shift_x += pan_rate
              popup_yes.change_position(-pan_rate, 0)
              popup_no.change_position(-pan_rate, 0)
              for btn in special_buttons:
                btn.change_position(-pan_rate, 0)
          elif btn.id == -2:
            if shift_y + pan_rate > -421:
              glTranslatef(0., pan_rate, 0.)
              shift_y += -pan_rate
              popup_yes.change_position(0, -pan_rate)
              popup_no.change_position(0, -pan_rate)
              for btn in special_buttons:
                btn.change_position(0, -pan_rate)
          elif btn.id == -3:
            if shift_y + pan_rate < 256:
              glTranslatef(0., -pan_rate, 0.)
              shift_y += pan_rate
              popup_yes.change_position(0, pan_rate)
              popup_no.change_position(0, pan_rate)
              for btn in special_buttons:
                btn.change_position(0, pan_rate)
          elif btn.id == -5:
            if shift_x - pan_rate > -376:
              glTranslatef(-pan_rate, 0., 0.)
              shift_x += -pan_rate
              popup_yes.change_position(pan_rate, 0)
              popup_no.change_position(pan_rate, 0)
              for btn in special_buttons:
                btn.change_position(pan_rate, 0)

      if popup_yes.on_button(x, y, shift_x, -shift_y) and popup_yes.is_pressed:
        path_declared = True
        print( "Starting at " , int_to_building(start) , ", ending at ", int_to_building(finish) )
        # print( start , " " , finish )

        # compute shortest path here
        previous = global_neighbor.dijkstra_compute_paths(start, global_map_b.getDistMatrix())
        path = global_neighbor.dijkstra_get_shortest_path_to(finish, previous)

        no_of_segments = len(path)
        global_path.fill_path(global_map_a, path)
        global_path.display()
        global_path.draw()
        start = finish = -1
        popup_yes.is_pressed = False
        redraw_window = True
        prefer_indoors = False
        avoid_stairs = False
        global_map_b.load_map_calc_weighted(ENTIRE_MAP_PATH, prefer_indoors, avoid_stairs, False, False)

      if popup_no.on_button(x, y, shift_x, -shift_y) and popup_no.is_pressed:
        start = finish = -1
        popup_no.is_pressed = False
        redraw_window = True

    glutPostRedisplay()


def mouse_motion(x, y):
  for btn in normal_buttons:
    if btn.is_pressed:
      ...
    elif btn.on_button(x, y):
      print(btn)
      btn.over_button = True
    else:
      btn.over_button = False
  for btn in special_buttons:
    if btn.is_pressed:
      ...
    elif btn.on_button(x, y):
      btn.over_button = True
    else:
      btn.over_button = False
  if popup_yes.is_pressed:
    ...
  elif popup_no.is_pressed:
    ...
  for node in global_map_a:
    if node.is_pressed():
      ...
  glutPostRedisplay()


def special_keyboard(key, x, y):
  global shift_x, shift_y
  if key == GLUT_KEY_LEFT:
    if (shift_x + pan_rate < 500):
      glTranslatef(pan_rate, 0., 0.)
      shift_x += pan_rate
      popup_yes.change_position(-pan_rate, 0)
      popup_no.change_position(-pan_rate, 0)
      for btn in special_buttons:
        btn.change_position(-pan_rate, 0)

  elif key == GLUT_KEY_RIGHT:
    if (shift_x - pan_rate > -374):
      glTranslatef(-pan_rate, 0., 0.)
      shift_x += -pan_rate
      # print( shift_x )
      popup_yes.change_position(pan_rate, 0)
      popup_no.change_position(pan_rate, 0)
      for btn in special_buttons:
        btn.change_position(pan_rate, 0)

  elif key == GLUT_KEY_UP:
    if (shift_y + pan_rate > -421):
      glTranslatef(0., pan_rate, 0.)
      shift_y += -pan_rate
      popup_yes.change_position(0, -pan_rate)
      popup_no.change_position(0, -pan_rate)
      for btn in special_buttons:
        btn.change_position(0, -pan_rate)

  elif key == GLUT_KEY_DOWN:
    if (shift_y + pan_rate < 256):
      glTranslatef(0., -pan_rate, 0.)
      shift_y += pan_rate
      # print( shift_y )
      popup_yes.change_position(0, pan_rate)
      popup_no.change_position(0, pan_rate)
      for btn in special_buttons:
        btn.change_position(0, pan_rate)

  glutPostRedisplay()


def reshape(w, h):
  glViewport(0, 0, w, h)
  WIDTH = w
  HEIGHT = h
  glMatrixMode(GL_PROJECTION)
  glLoadIdentity()
  glOrtho(0, WIDTH - 1, HEIGHT - 1, 0, -1, 1)


# the init function sets up the graphics card to draw properly
def init():
  # clear the window to black
  glClearColor(0.0, 0.0, 0.0, 1.0)
  glClear(GL_COLOR_BUFFER_BIT)

  # set up the coordinate system:  number of pixels along x and y
  glMatrixMode(GL_PROJECTION)
  glLoadIdentity()
  glOrtho(0., WIDTH - 1, HEIGHT - 1, 0., -1.0, 1.0)

  # allow blending (for transparent textures)
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
  glEnable(GL_BLEND)

  # welcome message
  print("Welcome to Ole Compass.")


def init_gl_window():
  glutInit(sys.argv)
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE)
  glutInitWindowSize(WIDTH, HEIGHT)
  glutInitWindowPosition(100, 100)
  glutCreateWindow(programName)
  init()

  texturePath = MAP_TEXTURE_PATH
  # campusmap = loadTexture(texturePath) # key to textures:  load them!

  glutDisplayFunc(drawWindow)
  glutReshapeFunc(reshape)
  glutSpecialFunc(special_keyboard)
  glutMouseFunc(mouse)
  glutMotionFunc(mouse_motion)
  glutMainLoop()

def main():
  global NodeList
  global_map_a.load_map_calc(ENTIRE_MAP_PATH)
  NodeList = deepcopy(global_map_a.nodes)
  load_normal_buttons(NORMAL_BUTTONS_PATH)
  load_special_buttons(SPECIAL_BUTTONS_PATH)
  global_map_b.load_map_calc_weighted(ENTIRE_MAP_PATH, prefer_indoors, avoid_stairs, False, False)
  init_gl_window()

if __name__ == '__main__':
  main()
