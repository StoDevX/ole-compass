from OpenGL.GL import glBegin, glRasterPos2f, glColor3f, glVertex2f, glEnd, GL_POLYGON
from OpenGL.GLUT import glutBitmapCharacter, GLUT_BITMAP_9_BY_15, GLUT_DEPTH, glutSwapBuffers, glutInit, glutInitDisplayMode, glutCreateWindow, glutDisplayFunc, glutMainLoop
from helpers import draw_text

class Button(object):
    def __init__(self, x=0, y=0, width=0, height=0, id='00', name='name'):
        self.x = int(x)
        self.y = int(y)
        self.width = int(width)
        self.height = int(height)
        self.id = id
        self.name = name
        self.is_pressed = False
        self.over_button = False

    def change_position(self, x, y):
        self.x = int(x)
        self.y = int(y)

    def __str__(self):
        return "{name} {x} {y}".format(self)


    def draw(self, r=0.1, g=0.1, b=0.1):
        glColor3f(.8, .8, .8)
        glBegin(GL_POLYGON)
        glVertex2f(self.x - 3, self.y - 3)                            # upper left
        glVertex2f(self.x - 3, self.y + self.height + 3)              # lower left
        glVertex2f(self.x + self.width + 3, self.y + self.height + 3) # lower right
        glVertex2f(self.x + self.width + 3, self.y - 3)               # upper right
        glEnd()
        if self.is_pressed:
            glColor3f(1., 1., 0.)
        elif self.over_button:
            glColor3f(.5, .5, .5)
        else:
            glColor3f(r, g, b)
        glBegin(GL_POLYGON)
        glVertex2f(self.x, self.y)                  # upper left
        glVertex2f(self.x, self.y + self.height)         # lower left
        glVertex2f(self.x + self.width, self.y + self.height) # lower right
        glVertex2f(self.x + self.width, self.y)          # upper right
        glEnd()
        glColor3f(1., 1., 1.)
        draw_text(self.x, self.y, self.name)


    def on_button(self, x, y, shift_factor_x=0, shift_factor_y=0):
        return (x >= self.x + shift_factor_x and
                y >= self.y + shift_factor_y and
                x <= self.x + shift_factor_x + width and
                y <= self.y + shift_factor_y + height)
