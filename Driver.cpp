#include <fstream>
#include <iostream>
using namespace std;
#include "Neighbor.h"
#include "Point.h"

#include <algorithm>
#include <iterator>
#include <list>
#include <set>
#include <string>
#include <utility> // for pair
#include <vector>

void loadMap(const char *filename, adjacency_list_t &vec, int &numNodes,
             Neighbor *&Nodes) {
  ifstream g(filename);
  if (!g.good()) {
    cerr << "Warning: Unable to open " << filename << ", ignoring it." << endl;
    return;
  }
  int x, y;
  g >> numNodes;
  vec.resize(numNodes);
  Nodes = new Neighbor[numNodes];
  int numConnections, vertex, weight;
  for (int i = 0; i < numNodes; ++i) {
    g >> x >> y;
    Nodes[i].setLocation(x, y);
    g >> numConnections;
    for (int j = 0; j < numConnections; ++j) {
      g >> vertex >> weight;
      vec[i].push_back(Neighbor(vertex, weight));
    }
  }
}
