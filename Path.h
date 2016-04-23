#ifndef _Path_
#define _Path_
#include "Map.h"
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;
#include "Neighbor.h"
#include "Point.h"

#include "opengl.h"

class Path: public Map{
protected:
  int distance;
  unsigned int pathsize;
 public:
  Path();
  void fillPath(Map, list<int>);
  void emptyPath();
  void display();
  int getDistance();
  unsigned int getPathsize();
  void setDistance(int);
  void setPathsize(unsigned int);
  void draw();
};

#endif // _Path_
