#include "Map.h"

void drawText(double, double, std::string);

Map::Map() {
  size = 0;
  nodes = 0;
}

int Map::getSize() { return size; }

adjacency_list_t Map::getDistMatrix() { return distMatrix; }

void Map::loadMap(std::string filename) {
  ifstream g(filename);
  if (!g.good()) {
    std::cerr << "Warning: Unable to open " << filename << ", ignoring it." << std::endl;
    return;
  }
  int x, y;
  g >> size;
  distMatrix.resize(size);
  nodes = new Neighbor[size];
  std::string buffer;
  int numConnections, vertex, weight;
  for (int i = 0; i < size; ++i) {
    g >> buffer;
    nodes[i].setID(buffer);
    g >> x >> y;
    nodes[i].setLocation(x, y);
    g >> numConnections;
    for (int j = 0; j < numConnections; ++j) {
      g >> vertex >> weight;
      distMatrix[i].push_back(Neighbor(vertex, weight));
    }
  }
}

void Map::loadMapCalc(std::string filename) {
  ifstream g(filename);
  if (!g.good()) {
    std::cerr << "Warning: Unable to open " << filename << ", ignoring it." << std::endl;
    return;
  }
  g >> size;
  distMatrix.resize(size);
  nodes = new Neighbor[size];
  int x, y;
  Point a, b;
  double dist;
  bool visib;
  std::string buffer;
  int numConnections, vertex;
  for (int i = 0; i < size; ++i) {
    g >> buffer;
    nodes[i].setID(buffer);
    g >> x >> y;
    nodes[i].setLocation(x, y);
    g >> visib;
    nodes[i].setVisibility(visib);
  }
  for (int i = 0; i < size; ++i) {
    g >> buffer;
    g >> numConnections;
    a = nodes[i].getLocation();
    for (int j = 0; j < numConnections; ++j) {
      g >> vertex;
      g >> x;
      b = nodes[vertex].getLocation();
      dist = a.distance(b);
      distMatrix[i].push_back(Neighbor(vertex, dist));
    }
  }
}

void Map::loadMapCalcWeighted(std::string filename, bool outdoor, bool stairs,
                              bool blacktop, bool paths) {
  ifstream g(filename);
  if (nodes != 0) {
    delete[] nodes;
  }
  if (!g.good()) {
    std::cerr << "Warning: Unable to open " << filename << ", ignoring it." << std::endl;
    return;
  }
  g >> size;
  distMatrix.clear();
  distMatrix.resize(size);
  nodes = new Neighbor[size];
  int x, y;
  int connectType;
  Point a, b;
  double dist;
  bool visib;
  std::string buffer;
  int numConnections, vertex;
  for (int i = 0; i < size; ++i) {
    g >> buffer;
    nodes[i].setID(buffer);
    g >> x >> y;
    nodes[i].setLocation(x, y);
    g >> visib;
    nodes[i].setVisibility(visib);
  }
  for (int i = 0; i < size; ++i) {
    g >> buffer;
    g >> numConnections;
    a = nodes[i].getLocation();
    for (int j = 0; j < numConnections; ++j) {
      g >> vertex;
      g >> connectType;
      b = nodes[vertex].getLocation();
      dist = a.distance(b);
      if (outdoor && connectType == 0) {
        dist *= 2;
      }
      if (stairs && connectType == 2) {
        dist *= 100;
      }
      if (blacktop && connectType == 3) {
        dist *= 20;
      }
      if (paths && connectType == 4) {
        dist *= 30;
      }
      distMatrix[i].push_back(Neighbor(vertex, dist));
    }
  }
}

Neighbor Map::operator[](int i) {
  if (i < 0) {
    cout << "Negative node - setting i=0. ";
    i = 0;
  }
  if (i > size) {
    cout << "Node greater than size. Setting i=size-1. ";
    i = size - 1;
  }
  return nodes[i];
}

void Map::listNodes() {
  for (int i = 0; i < size; ++i) {
    cout << nodes[i].getID() << ": ";
    nodes[i].getLocation().display();
    cout << std::endl;
  }
}

void Map::nodeDraw(bool drawAll, int begin, int end) {
  glColor3f(1., 1., 1.);
  double radius = 12;
  const int NUM_DIVS = 50;
  int x, y;
  std::string buffer;
  for (int i = 0; i < size; ++i) {
    if (nodes[i].getVisibility() || drawAll) {
      glColor3f(.4, .4, .8);
      if (i == begin || i == end) {
        glColor3f(0.2, 0.95, 0.7);
      }
      x = nodes[i].getLocation().x;
      y = nodes[i].getLocation().y;

      glBegin(GL_TRIANGLE_FAN);
      glVertex2d(x, y);
      for (int i = 0; i < NUM_DIVS; ++i)
        glVertex2d(x + radius * cos(i * 2 * M_PI / (NUM_DIVS - 1)),
                   y + radius * sin(i * 2 * M_PI / (NUM_DIVS - 1)));
      glEnd();

      glColor3f(0., 0., 0.);
      buffer = nodes[i].getID();
      drawText(x - 8, y + 6, buffer);
    }
  }
}

void Map::drawConnections(std::string filename) {
  ifstream g(filename);
  if (!g.good()) {
    std::cerr << "Warning: Unable to open " << filename << ", ignoring it." << std::endl;
    return;
  }
  int sizeSave = size;
  g >> size;
  Neighbor *nodesSave = nodes;
  nodes = new Neighbor[size];
  int x, y;
  int connectType;
  Point one, two;
  bool visib;
  std::string buffer;
  int numConnections, vertex;
  glColor3f(1., 0., 0.);
  for (int i = 0; i < size; ++i) {
    g >> buffer;
    nodes[i].setID(buffer);
    g >> x >> y;
    nodes[i].setLocation(x, y);
    g >> visib;
    nodes[i].setVisibility(visib);
  }
  for (int i = 0; i < size; ++i) {
    g >> buffer;
    g >> numConnections;
    one = nodes[i].getLocation();
    for (int j = 0; j < numConnections; ++j) {
      g >> vertex;
      g >> connectType;
      two = nodes[vertex].getLocation();
      if (i > vertex || true)
        rectangleDraw(one, two, 1, connectType);
    }
  }
  size = sizeSave;
  nodes = nodesSave;
}

void Map::rectangleDraw(Point a, Point b, double height, int pathType) {
  if (a.x == b.x) {
    a.x += .001;
  }
  double deltax, deltay, ax, bx, ay, by;
  float x1, x2, x3, x4, y1, y2, y3, y4;
  bx = b.x;
  by = b.y;
  ax = a.x;
  ay = a.y;

  deltax = height * sin(atan((by - ay) / (bx - ax)));
  deltay = -height * cos(atan((by - ay) / (bx - ax)));

  x1 = ax + deltax;
  x2 = ax - deltax;
  x3 = bx + deltax;
  x4 = bx - deltax;

  y1 = ay + deltay;
  y2 = ay - deltay;
  y3 = by + deltay;
  y4 = by - deltay;

  glColor3f(1., 0., 0.);
  if (pathType == 0) {
    glColor3f(1., 0., 0.);
  }
  if (pathType == 1) {
    glColor3f(1., 1., 1.);
  }
  if (pathType == 2) {
    glColor3f(0., 1., 0.);
  }
  if (pathType == 3) {
    glColor3f(0., 0., 1.);
  }
  if (pathType == 4) {
    glColor3f(.5, .5, .5);
  }

  glBegin(GL_POLYGON);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glVertex2f(x4, y4);
  glVertex2f(x3, y3);
  glEnd();
}

Neighbor *Map::equateNodes(Neighbor *other) {
  for (int i = 0; i < size; i++) {
    other[i] = nodes[i];
  }
  return other;
}

Map::~Map() {}
