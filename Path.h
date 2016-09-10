#ifndef _Path_
#define _Path_
#include "Map.h"
#include "Neighbor.h"
#include "Point.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "opengl.h"

class Path : public Map {
protected:
  int distance;
  size_t pathsize;

public:
  Path();
  void fillPath(Map, std::list<int>);
  void emptyPath();
  void display();
  int getDistance();
  size_t getPathsize();
  void setDistance(int);
  void setPathsize(unsigned int);
  void draw();
};

#endif // _Path_
