#ifndef _Button_
#define _Button_
#include "opengl.h"
#include <iostream>
#include <string>
using namespace std;

class Button {
protected:
  int x_pos;
  int y_pos;
  int width;
  int height;

  string name;
  bool isPressed;
  bool overButton;
  int ID;

public:
  Button();
  Button(istream &istr);
  Button(int x, int y, int w, int h, int id, string str);
  ~Button();

  void draw();
  void draw(int r, int g, int b);
  void draw(double r, double g, double b);

  void drawText();
  void display();

  bool onButton(int x, int y, int ShiftFactorX, int ShiftFactorY);

  void setButtonIsPressed(bool newVal);
  void setOverButton(bool newVal);

  string getName();
  int getID();
  bool getButtonIsPressed();

  void changePosition(int x, int y);
  void changeName(string newName);
};
#endif
