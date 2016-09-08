#include "Point.h"

Point::Point() {
  x = 0;
  y = 0;
}

Point::Point(double newx, double newy) {
  x = newx;
  y = newy;
}

void Point::display() { std::cout << "(" << x << "," << y << ")"; }

Point &Point::operator=(const Point &p) {
  x = p.x;
  y = p.y;
  return *this;
}

double Point::distance(Point a) {
  return sqrt((a.x - x) * (a.x - x) + (a.y - y) * (a.y - y));
}
