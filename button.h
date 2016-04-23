#ifndef _Button_
#define _Button_
#include <iostream>
#include "opengl.h"
#include <cmath>
#include <string>
using namespace std;

class Button
{
 protected:
  int x_pos;
  int y_pos;
  int width;
  int height;
  char* name;
  bool ButtonIsPressed;
  bool overButton;
  int ID;
 public:
  Button();
  Button(istream& istr);
  Button(int x, int y, int w, int h, int id, const char* str);
  void drawText();
  void draw();
  void draw(int r, int g, int b);
  void display();
  ~Button();
  bool onButton(int x, int y, int ShiftFactorX, int ShiftFactorY);
  void setButtonIsPressed(bool newVal);
  void setoverButton(bool newVal);
  char* getName();
  int getID();
  bool getButtonIsPressed();
  void changePosition(int x, int y);
  void changeName(const char* newName);
};
#endif
