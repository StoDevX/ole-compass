#include "button.h"

Button::Button() {
  x_pos = 0;
  y_pos = 0;
  width = 0;
  height = 0;
}

Button::Button(istream &istr) {
  istr >> x_pos;
  istr >> y_pos;
  istr >> width;
  istr >> height;
  char name[10];
  for (int i = 0; i < 10; i++)
    istr >> name[i];
}

Button::Button(int x, int y, int w, int h, int id, const char *str) {
  x_pos = x;
  y_pos = y;
  width = w;
  height = h;
  ID = id;
  int length = 0;
  while (str[length] != 0) {
    length++;
  }
  name = new char[length + 1];
  for (int i = 0; i < length + 1; i++) {
    name[i] = str[i];
  }
  overButton = false;
  ButtonIsPressed = false;
}

void Button::drawText() {
  glRasterPos2f(x_pos, y_pos + 10);
  int length = 0;
  while (name[length] != 0)
    ++length;
  for (int i = 0; i < length; i++)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, name[i]);
}

void Button::draw() {
  glColor3f(.8, .8, .8);
  glBegin(GL_POLYGON);
  glVertex2f(x_pos - 3, y_pos - 3);                  // upper left
  glVertex2f(x_pos - 3, y_pos + height + 3);         // lower left
  glVertex2f(x_pos + width + 3, y_pos + height + 3); // lower right
  glVertex2f(x_pos + width + 3, y_pos - 3);          // upper right
  glEnd();
  if (ButtonIsPressed)
    glColor3f(1., 1., 0.);
  else if (overButton)
    glColor3f(.5, .5, .5);
  else
    glColor3f(.1, .1, .1);
  glBegin(GL_POLYGON);
  glVertex2f(x_pos, y_pos);                  // upper left
  glVertex2f(x_pos, y_pos + height);         // lower left
  glVertex2f(x_pos + width, y_pos + height); // lower right
  glVertex2f(x_pos + width, y_pos);          // upper right
  glEnd();
  glColor3f(1., 1., 1.);
  drawText();
}

void Button::draw(int r, int g, int b) {
  glColor3f(.8, .8, .8);
  glBegin(GL_POLYGON);
  glVertex2f(x_pos - 3, y_pos - 3);                  // upper left
  glVertex2f(x_pos - 3, y_pos + height + 3);         // lower left
  glVertex2f(x_pos + width + 3, y_pos + height + 3); // lower right
  glVertex2f(x_pos + width + 3, y_pos - 3);          // upper right
  glEnd();
  if (ButtonIsPressed)
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

void Button::display() { cout << name << " " << x_pos << " " << y_pos << endl; }

bool Button::onButton(int x, int y, int ShiftFactorX, int ShiftFactorY) {
  return x >= x_pos + ShiftFactorX && y >= y_pos + ShiftFactorY &&
         x <= x_pos + ShiftFactorX + width &&
         y <= y_pos + ShiftFactorY + height;
}

void Button::setButtonIsPressed(bool newVal) { ButtonIsPressed = newVal; }

void Button::setoverButton(bool newVal) { overButton = newVal; }

char *Button::getName() { return name; }

int Button::getID() { return ID; }

bool Button::getButtonIsPressed() { return ButtonIsPressed; }

void Button::changePosition(int x, int y) {
  x_pos += x;
  y_pos += y;
}

Button::~Button() { delete[] name; }

void Button::changeName(const char *newName) {
  int length = 0;
  while (newName[length] != 0) {
    length++;
  }
  name = new char[length + 1];
  for (int i = 0; i < length + 1; i++) {
    name[i] = newName[i];
  }
}
