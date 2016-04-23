#ifndef _Map_
#define _Map_
#include "Neighbor.h"
#include "Point.h"

#include "opengl.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Map {
private:
  adjacency_list_t distMatrix;

protected:
  int size;
  Neighbor *nodes;
  void rectangleDraw(Point, Point, double, int);

public:
  Map();
  int getSize();
  void loadMap(const char *);
  void loadMapCalc(const char *);
  void loadMapCalcWeighted(const char *, bool, bool, bool, bool);
  adjacency_list_t getDistMatrix();
  Neighbor operator[](int);
  void listNodes();
  void nodeDraw(bool, int, int);
  void drawConnections(const char *filename);
  Neighbor *equateNodes(Neighbor *);
  ~Map();
};

#endif // _Map_
