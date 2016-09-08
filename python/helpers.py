from OpenGL.GL import glBegin, glEnd, glVertex2f, glColor3f, glRasterPos2f
from OpenGL.GLUT import glutBitmapCharacter, GLUT_BITMAP_9_BY_15
from math import sin, cos, atan
from ctypes import c_int

def draw_rectangle(a, b, height, pathType):
    bx, by = b
    ax, ay = a

    if ax is bx:
      ax += .001

    deltax = height * sin(atan((by - ay) / (bx - ax)))
    deltay = -height * cos(atan((by - ay) / (bx - ax)))

    x1 = ax + deltax
    x2 = ax - deltax
    x3 = bx + deltax
    x4 = bx - deltax

    y1 = ay + deltay
    y2 = ay - deltay
    y3 = by + deltay
    y4 = by - deltay

    glColor3f(1., 0., 0.)
    if pathType is 0:
      glColor3f(1., 0., 0.)
    if pathType is 1:
      glColor3f(1., 1., 1.)
    if pathType is 2:
      glColor3f(0., 1., 0.)
    if pathType is 3:
      glColor3f(0., 0., 1.)
    if pathType is 4:
      glColor3f(.5, .5, .5)

    glBegin(GL_POLYGON)
    glVertex2f(x1, y1)
    glVertex2f(x2, y2)
    glVertex2f(x4, y4)
    glVertex2f(x3, y3)
    glEnd()


def draw_text(x, y, text):
    glRasterPos2f(x, y + 10)
    for ch in text:
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c_int(ord(ch)))


def draw_box(x, y, width, height):
    glBegin(GL_POLYGON)
    glVertex2f(x, y);                  # upper left
    glVertex2f(x, y + height);         # lower left
    glVertex2f(x + width, y + height); # lower right
    glVertex2f(x + width, y);          # upper right
    glEnd()
