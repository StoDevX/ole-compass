#ifndef _Neighbor_
#define _Neighbor_
#include "Point.h"
#include <iostream>
using namespace std;
#include "texture.h"
#include <cmath>

#include <algorithm>
#include <iterator>
#include <list>
#include <set>
#include <string>
#include <utility> // for pair
#include <vector>

#include "opengl.h"

class Neighbor;

typedef vector<vector<Neighbor>> adjacency_list_t;

class Neighbor {
protected:
  Point loc;
  string id;
  int target;
  double weight;
  bool visible;
  bool overNode;
  bool NodeIsPressed;

public:
  Neighbor(int, double);
  Neighbor();
  void setLocation(int, int);
  void setID(string);
  void setVisibility(bool);
  string getID();
  Point getLocation();
  bool getVisibility();
  void DijkstraComputePaths(int source, const adjacency_list_t &,
                            vector<double> &, vector<int> &);
  list<int> DijkstraGetShortestPathTo(int, const vector<int> &);
  void display();
  bool onNode(int x, int y, int ShiftFactorX, int ShiftFactorY);
  void setNodeIsPressed(bool newVal);
  void setoverNode(bool newVal);
  bool getNodeIsPressed();
};

#endif // _Node_
