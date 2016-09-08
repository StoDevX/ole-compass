#ifndef _TEXTURE_
#define _TEXTURE_

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "opengl.h"
#include <cassert>
#include <cmath>   // for cos, sin
#include <cstdlib> // for exit
#include <cstring> // for strcmp

void drawTexture(int texNum, double x, double y, double width, double height,
                 double alpha = 1.0, double rotationAngle = 0.0);
void drawTexture(int texNum, double *arr, double alpha = 1.0);
int loadTexture(std::string filename);

#endif // _TEXTURE_
