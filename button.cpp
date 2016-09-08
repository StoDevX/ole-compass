#include "button.h"

Button::Button() {
  x_pos = 0;
  y_pos = 0;
  width = 0;
  height = 0;
}

Button::Button(std::istream &istr) {
  istr >> x_pos;
  istr >> y_pos;
  istr >> width;
  istr >> height;
  istr >> name;
}

Button::Button(int x, int y, int w, int h, int id, std::string str) {
  x_pos = x;
  y_pos = y;
  width = w;
  height = h;
  ID = id;
  name = str;
  overButton = false;
  isPressed = false;
}

void Button::drawText() {
  glRasterPos2f(x_pos, y_pos + 10);
  for (char ch : name)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ch);
}

void Button::draw() { draw(.1, .1, .1); }

void Button::draw(int r, int g, int b) {
  draw((double)r, (double)g, (double)b);
}

void Button::draw(double r, double g, double b) {
  glColor3f(.8, .8, .8);
  glBegin(GL_POLYGON);
  glVertex2f(x_pos - 3, y_pos - 3);                  // upper left
  glVertex2f(x_pos - 3, y_pos + height + 3);         // lower left
  glVertex2f(x_pos + width + 3, y_pos + height + 3); // lower right
  glVertex2f(x_pos + width + 3, y_pos - 3);          // upper right
  glEnd();
  if (isPressed)
    glColor3f(1., 1., 0.);
  else if (overButton)
    glColor3f(.5, .5, .5);
  else
    glColor3f(r, g, b);
  glBegin(GL_POLYGON);
  glVertex2f(x_pos, y_pos);                  // upper left
  glVertex2f(x_pos, y_pos + height);         // lower left
  glVertex2f(x_pos + width, y_pos + height); // lower right
  glVertex2f(x_pos + width, y_pos);          // upper right
  glEnd();
  glColor3f(1., 1., 1.);
  drawText();
}

void Button::display() {
  std::cout << name << " " << x_pos << " " << y_pos << std::endl;
}

bool Button::onButton(int x, int y, int ShiftFactorX, int ShiftFactorY) {
  return x >= x_pos + ShiftFactorX && y >= y_pos + ShiftFactorY &&
         x <= x_pos + ShiftFactorX + width &&
         y <= y_pos + ShiftFactorY + height;
}

void Button::setButtonIsPressed(bool newVal) { isPressed = newVal; }

void Button::setOverButton(bool newVal) { overButton = newVal; }

std::string Button::getName() { return name; }

int Button::getID() { return ID; }

bool Button::getButtonIsPressed() { return isPressed; }

void Button::changePosition(int x, int y) {
  x_pos += x;
  y_pos += y;
}

Button::~Button() {}

void Button::changeName(std::string newName) { name = newName; }
