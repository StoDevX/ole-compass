#ifndef _TEXTURE_
#define _TEXTURE_

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;
#include "opengl.h"
#include <cassert>
#include <cstring>  // for strcmp
#include <cstdlib>  // for exit
#include <cmath>    // for cos, sin

void drawTexture(int texNum, double x, double y, double width, double height, double alpha=1.0, double rotationAngle=0.0);
void drawTexture(int texNum, double *arr, double alpha=1.0);
int loadTexture(const char *filename);

#endif // _TEXTURE_
