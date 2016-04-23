#ifndef _Map_
#define _Map_
#include "Point.h"
#include "Neighbor.h"

#include <vector>
#include <list>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "opengl.h"
using namespace std;


class Map{
 private:
adjacency_list_t distMatrix;
 protected:
  int size;
  Neighbor* nodes;
  void rectangleDraw(Point,Point,double,int);
 public:
  Map();
  int getSize();
  void loadMap(const char*);
  void loadMapCalc(const char*);
  void loadMapCalcWeighted(const char *, bool, bool, bool, bool);
  adjacency_list_t getDistMatrix();
  Neighbor operator[](int);
  void listNodes();
  void nodeDraw(bool,int,int);
  void drawConnections(const char *filename);
  Neighbor* equateNodes(Neighbor*);
  ~Map();
};





#endif // _Map_
