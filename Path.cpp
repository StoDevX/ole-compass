#include "Path.h"

void drawText(double, double, std::string);

Path::Path() : Map() {
  distance = 0;
  pathsize = 0;
}

void Path::fillPath(Map m, std::list<int> route) {
  pathsize = route.size();
  int *array = new int[pathsize];
  nodes = new Neighbor[pathsize];
  copy(route.begin(), route.end(), array);
  for (unsigned int i = 0; i < pathsize; ++i) {
    nodes[i] = m[array[i]];
  }
  distance = 0;
  Point a;
  for (unsigned int i = 0; i < pathsize - 1; ++i) {
    a = nodes[i].getLocation();
    distance += a.distance(nodes[i + 1].getLocation());
  }
}

void Path::emptyPath() {
  pathsize = 0;
  if (nodes != 0) {
    delete[] nodes;
  }
  nodes = 0;
  distance = 0;
}

void Path::display() {
  std::cout << "Distance from " << nodes[0].getID() << " to "
            << nodes[pathsize - 1].getID() << ": ";
  std::cout << distance << std::endl;
  std::cout << "Route: ";
  for (unsigned int i = 0; i < pathsize; ++i) {
    std::cout << nodes[i].getID() << " ";
  }
  std::cout << std::endl;
}

int Path::getDistance() { return distance; }

size_t Path::getPathsize() { return pathsize; }

void Path::setDistance(int dist) { distance = dist; }

void Path::setPathsize(unsigned int length) { pathsize = length; }

void Path::draw() {
  Point one, two;
  if (pathsize == 0) {
    ++pathsize;
  }
  for (unsigned int i = 0; i < pathsize - 1; ++i) {
    one = nodes[i].getLocation();
    two = nodes[i + 1].getLocation();
    rectangleDraw(one, two, 3, 0);
  }
}
