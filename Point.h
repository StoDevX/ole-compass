#ifndef _Point_
#define _Point_

#include <iostream>
using namespace std;
#include <cmath>


struct Point{
  double x;
  double y;
  Point();
  Point(double newx, double newy);
  void display();
  Point& operator=(const Point&);
  double distance(Point);
};



#endif // _Point_
