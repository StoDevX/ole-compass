#include <fstream>
#include <iostream>
#include "opengl.h"
#include "texture.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include "Map.h"
#include "Neighbor.h"
#include "Point.h"

#include <list>
#include <string>
#include <vector>

#include <algorithm>
#include <iterator>
#include <set>
#include <utility>

#include "Path.h"
#include "button.h"

bool mouseIsDragging = false;
bool selectionFinished = false;
bool redrawWindow = false;
bool pathDeclared = false;

bool preferIndoors = false;
bool avoidStairs = false;
bool drawInvisibles = false;
bool drawNodes = true;
bool drawAll = true;
bool drawGrid = false;

Neighbor a(1, 1);
Map A;
Map B;
Map C;
int *arr;
Path b;

int WIDTH = 940;  // width of the user window (640 + 80) 720 1080
int HEIGHT = 640; // height of the user window (480 + 60) 540 720
char programName[] = "OleCompass";
int campusmap; // texture IDs

int shiftx = 0;
int shifty = 0;
int panRate = 50;

int no_of_segments;

int Start = -1;
int Finish = -1;

// button info
int numButtons;
Button *ButtonList[50];

int numSpecialButtons;
Button *SpecialButtonList[50];
Neighbor NodeList[228];

Button PopupYes(200, 300, 60, 30, -1, "Yes");
Button PopupNo(300, 300, 60, 30, -1, "No");

void loadButtons(std::string file) {
  ifstream infile(file);
  if (!infile.good()) {
    std::cerr << "Warning: Unable to open " << file << ", ignoring it." << std::endl;
  }
  infile >> numButtons;

  for (int i = 0; i < numButtons; i++) {
    int x, y, w, h, id;
    infile >> x >> y >> w >> h >> id;
    std::string ButtonName;
    infile >> ButtonName;
    ButtonList[i] = new Button(x, y, w, h, id, ButtonName);
  }
}

void loadSpecialButtons(std::string file) {
  ifstream infile(file);
  if (!infile.good()) {
    std::cerr << "Warning: Unable to open " << file << ", ignoring it." << std::endl;
  }
  infile >> numSpecialButtons;

  for (int i = 0; i < numSpecialButtons; i++) {
    int x, y, w, h, id;
    infile >> x >> y >> w >> h >> id;
    std::string ButtonName;
    infile >> ButtonName;
    SpecialButtonList[i] = new Button(x, y, w, h, id, ButtonName);
  }
}

std::string IntToBuilding(int x) {
  std::string temp = NodeList[x].getID();
  for (int i = 0; i < numButtons; i++) {
    if (x == ButtonList[i]->getID()) {
      temp = ButtonList[i]->getName();
    }
  }

  return temp;
}

void drawBox(double x, double y, double width, double height) {
  glBegin(GL_POLYGON);
  glVertex2f(x, y);                  // upper left
  glVertex2f(x, y + height);         // lower left
  glVertex2f(x + width, y + height); // lower right
  glVertex2f(x + width, y);          // upper right
  glEnd();
}

void drawBox(double *pos) { drawBox(pos[0], pos[1], pos[2], pos[3]); }

void drawText(double x, double y, std::string text) {
  glRasterPos2f(x, y);
  for (const char ch : text)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ch);
}

// DRAWING WINDOW

void drawWindow() {
  // clear the buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // draw stuff
  // The original resolution of the image is 1053 x 775. We can change the
  // effective "zoom" of the map by increasing or decreasing this resolution.
  // However, try to keep the ratio of width to height as close as possible to
  // maintain the aspect ratio of the map so it isn't stretched or anything.
  // texID,   x,y,    width, height
  drawTexture(campusmap, -600, -500, 1895, 1395);
  // drawing path
  // for (int i=0; i<no_of_segments;++i)

  // drawing the calculated path (if none, this is empty.)

  b.draw();

  // drawing ALL connections.

  // C.drawConnections("EntireMapCalc.txt");

  if (drawGrid) {
    C.drawConnections("EntireMapCalc.txt");
  }

  // drawing nodes
  if (drawAll) {
    if (drawNodes) {
      A.nodeDraw(drawInvisibles, Start, Finish);
      // C.drawConnections("EntireMapCalc.txt");
    }
  }

  // drawing buttons
  if (drawAll) {
    for (int i = 0; i < numButtons; i++) {
      ButtonList[i]->draw();
    }
  }

  for (int i = 0; i < numSpecialButtons; i++) {
    SpecialButtonList[i]->draw(0, 0, 100);
    if (SpecialButtonList[i]->getID() == -1) {
      SpecialButtonList[i]->draw(100, 0, 0);
    }
    if (SpecialButtonList[i]->getID() == -6) {
      if (preferIndoors == true) {
        SpecialButtonList[i]->changeName("Unweigh Indoors");
        SpecialButtonList[i]->draw(0, 0, 100);
      } else if (preferIndoors == false) {
        SpecialButtonList[i]->changeName("Weigh Indoors");
        SpecialButtonList[i]->draw(0, 0, 100);
      }
    }
    if (SpecialButtonList[i]->getID() == -8) {
      if (avoidStairs == true) {
        SpecialButtonList[i]->changeName("Include stairs");
        SpecialButtonList[i]->draw(0, 0, 100);
      } else if (avoidStairs == false) {
        SpecialButtonList[i]->changeName("Avoid stairs");
        SpecialButtonList[i]->draw(0, 0, 100);
      }
    }
  }
  glColor3f(1., 1., 1.);
  drawText(10 - shiftx, 95 + shifty, "Panning Rate");
  // popup display
  if (selectionFinished == true && pathDeclared == false) {
    glColor3f(.5, .5, .5); // white
    drawBox(100 - shiftx, 100 + shifty, 400, 300);
    glColor3f(0., 0., 0.); // text is black
    drawText(150 - shiftx, 200 + shifty, "Do you want to find the shortest");
    drawText(150 - shiftx, 220 + shifty, "path between");
    glColor3f(1., 0., 0.);
    drawText(150 - shiftx, 240 + shifty, IntToBuilding(Start));
    glColor3f(0., 0., 0.);
    drawText(150 - shiftx, 260 + shifty, "and");
    glColor3f(1., 0., 0.);
    drawText(150 - shiftx, 280 + shifty, IntToBuilding(Finish));
    // PopupYes.changePosition(-shiftx+panRate, shifty+panRate);
    // PopupNo.changePosition(-shiftx+panRate, shifty+panRate);
    PopupYes.draw();
    PopupNo.draw();
    if (PopupYes.getButtonIsPressed()) {
      selectionFinished = false;
    }
    if (PopupNo.getButtonIsPressed()) {
      selectionFinished = false;
    }
  }

  glutSwapBuffers();
}

void PrintPath() {
  if (Finish != -1) {
    std::cout << "Selection Finished!" << std::endl;
    selectionFinished = true;
    std::cout << selectionFinished << std::endl;
    // std::cout << "Starting at " << IntToBuilding(Start) << ", ending at " <<
    // IntToBuilding(Finish) << std::endl;
    // std::cout << Start << " " << Finish << std::endl;
  }
}

void FillPath(int x) {
  if (Start == -1) {
    Start = x;
  } else if (Start != -1) {
    if (Finish == -1) {
      Finish = x;
    }
  }
}

// the mouse function is called when a mouse button is pressed down or released
void mouse(int mouseButton, int state, int x, int y) {
  if (GLUT_LEFT_BUTTON == mouseButton) {
    if (GLUT_DOWN == state) {
      // This std::cout prints the current location of the mouse upon a click
      // std::cout << x-shiftx << ", " << y+shifty << std::endl;
      // std::cout << shiftx << ", " << shifty << std::endl;
      for (int i = 0; i < numButtons; i++) {
        if (ButtonList[i]->onButton(x, y, shiftx, -shifty)) {
          ButtonList[i]->setButtonIsPressed(true);
        } else {
          ButtonList[i]->setButtonIsPressed(false);
        }
      }
      for (int i = 0; i < A.getSize(); i++) {
        if (NodeList[i].onNode(x, y, shiftx, -shifty)) {
          NodeList[i].setNodeIsPressed(true);
        } else {
          NodeList[i].setNodeIsPressed(false);
        }
      }

      for (int i = 0; i < numSpecialButtons; i++) {
        if (SpecialButtonList[i]->onButton(x, y, shiftx, -shifty)) {
          SpecialButtonList[i]->setButtonIsPressed(true);
          for (int i = 0; i < numButtons; i++) {
            ButtonList[i]->setButtonIsPressed(false);
          }
          for (int i = 0; i < A.getSize(); i++) {
            NodeList[i].setNodeIsPressed(false);
          }
        } else {
          SpecialButtonList[i]->setButtonIsPressed(false);
        }
      }

      if (PopupYes.onButton(x, y, shiftx, -shifty)) {
        PopupYes.setButtonIsPressed(true);
      } else {
        PopupYes.setButtonIsPressed(false);
      }
      if (PopupNo.onButton(x, y, shiftx, -shifty)) {
        PopupNo.setButtonIsPressed(true);
      } else {
        PopupNo.setButtonIsPressed(false);
      }
    } else {
      for (int i = 0; i < numButtons; i++) {
        if (ButtonList[i]->onButton(x, y, shiftx, -shifty) &&
            ButtonList[i]->getButtonIsPressed()) {
          std::cout << ButtonList[i]->getName() << " selected" << std::endl;
          FillPath(ButtonList[i]->getID());
          PrintPath();
          ButtonList[i]->setButtonIsPressed(false);
        }
      }

      for (int i = 0; i < A.getSize(); i++) {
        if (NodeList[i].onNode(x, y, shiftx, -shifty) &&
            NodeList[i].getNodeIsPressed()) {
          std::cout << "Node " << i << " selected" << std::endl;
          FillPath(i);
          PrintPath();
          NodeList[i].setNodeIsPressed(false);
        }
      }

      // RESET BUTTON
      if (SpecialButtonList[0]->onButton(x, y, shiftx, -shifty) &&
          SpecialButtonList[0]->getButtonIsPressed()) {
        std::cout << "GLOBAL RESET" << std::endl;
        b.emptyPath();
        SpecialButtonList[0]->setButtonIsPressed(false);
        Start = Finish = -1;
        preferIndoors = false;
        selectionFinished = false;
        pathDeclared = false;
        avoidStairs = false;
        drawInvisibles = false;
        drawNodes = true;
        drawAll = true;
        drawGrid = false;
      }

      // PREFER INDOORS BUTTON
      if (SpecialButtonList[5]->onButton(x, y, shiftx, -shifty) &&
          SpecialButtonList[5]->getButtonIsPressed()) {
        if (preferIndoors == false) {
          std::cout << "Prefering indoors" << std::endl;
          preferIndoors = true;
          B.loadMapCalcWeighted("EntireMapCalc.txt", preferIndoors, avoidStairs,
                                false, false);
          SpecialButtonList[5]->setButtonIsPressed(false);
        } else if (preferIndoors == true) {
          std::cout << "Not prefering indoors" << std::endl;
          preferIndoors = false;
          B.loadMapCalcWeighted("EntireMapCalc.txt", preferIndoors, avoidStairs,
                                false, false);
          SpecialButtonList[5]->setButtonIsPressed(false);
        }
        // B.loadMapCalcWeighted("EntireMapCalc.txt", preferIndoors,
        // avoidStairs,false,false);
      }
      // else B.loadMapCalcWeighted("EntireMapCalc.txt", preferIndoors,
      // avoidStairs,false,false);

      // AVOID STAIRS BUTTON
      if (SpecialButtonList[7]->onButton(x, y, shiftx, -shifty) &&
          SpecialButtonList[7]->getButtonIsPressed()) {
        if (avoidStairs == true) {
          std::cout << "No longer avoiding stairs" << std::endl;
          avoidStairs = false;
          B.loadMapCalcWeighted("EntireMapCalc.txt", preferIndoors, avoidStairs,
                                false, false);
          SpecialButtonList[7]->setButtonIsPressed(false);
        } else if (avoidStairs == false) {
          std::cout << "Avoiding Stairs" << std::endl;
          avoidStairs = true;
          B.loadMapCalcWeighted("EntireMapCalc.txt", preferIndoors, avoidStairs,
                                false, false);
          SpecialButtonList[7]->setButtonIsPressed(false);
        }
      }

      // DRAW ALL BUTTON
      if (SpecialButtonList[6]->onButton(x, y, shiftx, -shifty) &&
          SpecialButtonList[6]->getButtonIsPressed()) {
        if (drawAll == false) {
          drawAll = true;
          SpecialButtonList[6]->setButtonIsPressed(false);
        } else if (drawAll == true) {
          drawAll = false;
          SpecialButtonList[6]->setButtonIsPressed(false);
        }
      }

      // DRAW HIDDEN BUTTON
      if (SpecialButtonList[8]->onButton(x, y, shiftx, -shifty) &&
          SpecialButtonList[8]->getButtonIsPressed()) {
        if (drawInvisibles == false) {
          drawInvisibles = true;
          SpecialButtonList[8]->setButtonIsPressed(false);
        } else if (drawInvisibles == true) {
          drawInvisibles = false;
          SpecialButtonList[8]->setButtonIsPressed(false);
        }
      }

      // DRAW NODES BUTTON
      if (SpecialButtonList[9]->onButton(x, y, shiftx, -shifty) &&
          SpecialButtonList[9]->getButtonIsPressed()) {
        if (drawNodes == false) {
          drawNodes = true;
          SpecialButtonList[9]->setButtonIsPressed(false);
        } else if (drawNodes == true) {
          drawNodes = false;
          SpecialButtonList[9]->setButtonIsPressed(false);
        }
      }

      // DRAW GRID BUTTON
      if (SpecialButtonList[10]->onButton(x, y, shiftx, -shifty) &&
          SpecialButtonList[10]->getButtonIsPressed()) {
        if (drawGrid == false) {
          drawGrid = true;
          SpecialButtonList[10]->setButtonIsPressed(false);
        } else if (drawGrid == true) {
          drawGrid = false;
          SpecialButtonList[10]->setButtonIsPressed(false);
        }
      }

      if (SpecialButtonList[11]->onButton(x, y, shiftx, -shifty) &&
          SpecialButtonList[11]->getButtonIsPressed()) {
        panRate += 5;
        SpecialButtonList[11]->setButtonIsPressed(false);
        std::cout << "Pan Rate changed to: " << panRate << std::endl;
      }
      if (SpecialButtonList[12]->onButton(x, y, shiftx, -shifty) &&
          SpecialButtonList[12]->getButtonIsPressed()) {
        panRate -= 5;
        SpecialButtonList[12]->setButtonIsPressed(false);
        std::cout << "Pan Rate changed to: " << panRate << std::endl;
      }

      // ON SCREEN PANNING BUTTONS
      for (int i = 0; i < 5; i++) {
        if (SpecialButtonList[i]->onButton(x, y, shiftx, -shifty) &&
            SpecialButtonList[i]->getButtonIsPressed()) {
          SpecialButtonList[i]->setButtonIsPressed(false);
          if (SpecialButtonList[i]->getID() == -4) {
            if (shiftx + panRate < 500) {
              glTranslatef(panRate, 0., 0.);
              shiftx += panRate;
              PopupYes.changePosition(-panRate, 0);
              PopupNo.changePosition(-panRate, 0);
              for (int i = 0; i < numSpecialButtons; i++) {
                SpecialButtonList[i]->changePosition(-panRate, 0);
              }
            }
          } else if (SpecialButtonList[i]->getID() == -2) {
            if (shifty + panRate > -421) {
              glTranslatef(0., panRate, 0.);
              shifty += -panRate;
              PopupYes.changePosition(0, -panRate);
              PopupNo.changePosition(0, -panRate);
              for (int i = 0; i < numSpecialButtons; i++) {
                SpecialButtonList[i]->changePosition(0, -panRate);
              }
            }
          } else if (SpecialButtonList[i]->getID() == -3) {
            if (shifty + panRate < 256) {
              glTranslatef(0., -panRate, 0.);
              shifty += panRate;
              PopupYes.changePosition(0, panRate);
              PopupNo.changePosition(0, panRate);
              for (int i = 0; i < numSpecialButtons; i++) {
                SpecialButtonList[i]->changePosition(0, panRate);
              }
            }
          } else if (SpecialButtonList[i]->getID() == -5) {
            if (shiftx - panRate > -376) {
              glTranslatef(-panRate, 0., 0.);
              shiftx += -panRate;
              PopupYes.changePosition(panRate, 0);
              PopupNo.changePosition(panRate, 0);
              for (int i = 0; i < numSpecialButtons; i++) {
                SpecialButtonList[i]->changePosition(panRate, 0);
              }
            }
          }
        }
      }

      if (PopupYes.onButton(x, y, shiftx, -shifty) &&
          PopupYes.getButtonIsPressed()) {
        pathDeclared = true;
        std::cout << "Starting at " << IntToBuilding(Start) << ", ending at "
             << IntToBuilding(Finish) << std::endl;
        // std::cout << Start << " " << Finish << std::endl;

        // compute shortest path here
        vector<double> min_distance;
        vector<int> previous;
        a.DijkstraComputePaths(Start, B.getDistMatrix(), min_distance,
                               previous);
        list<int> path = a.DijkstraGetShortestPathTo(Finish, previous);
        /*      std::cout << "Path : ";*/
        arr = new int[path.size()];
        copy(path.begin(), path.end(), arr);
        no_of_segments = path.size();
        /*
        std::cout << "Distance from " << Start << " to " << Finish <<": " <<
       min_distance[Finish] << std::endl;

        std::cout << "Path : ";
        copy(path.begin(), path.end(), ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
       for (unsigned int i = 0; i < path.size(); ++i)
          std::cout << arr[i] << " ";
          std::cout << std::endl;*/
        b.fillPath(A, path);
        b.display();
        b.draw();
        Start = Finish = -1;
        PopupYes.setButtonIsPressed(false);
        redrawWindow = true;
        preferIndoors = false;
        avoidStairs = false;
        B.loadMapCalcWeighted("EntireMapCalc.txt", preferIndoors, avoidStairs,
                              false, false);
      }

      if (PopupNo.onButton(x, y, shiftx, -shifty) &&
          PopupNo.getButtonIsPressed()) {
        Start = Finish = -1;
        PopupNo.setButtonIsPressed(false);
        redrawWindow = true;
      }
    }
    if (GLUT_RIGHT_BUTTON == mouseButton) {
    }
    glutPostRedisplay();
  }
}

void mouse_motion(int x, int y) {
  for (int i = 0; i < numButtons; i++) {
    if (ButtonList[i]->getButtonIsPressed()) {
    }
  }
  for (int i = 0; i < numSpecialButtons; i++) {
    if (SpecialButtonList[i]->getButtonIsPressed()) {
    }
  }
  if (PopupYes.getButtonIsPressed()) {
  } else if (PopupNo.getButtonIsPressed()) {
  }
  for (int i = 0; i < A.getSize(); i++) {
    if (NodeList[i].getNodeIsPressed()) {
    }
  }
  glutPostRedisplay();
}

void special_keyboard(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_LEFT:
    if (shiftx + panRate < 500) {
      glTranslatef(panRate, 0., 0.);
      shiftx += panRate;
      PopupYes.changePosition(-panRate, 0);
      PopupNo.changePosition(-panRate, 0);
      for (int i = 0; i < numSpecialButtons; i++) {
        SpecialButtonList[i]->changePosition(-panRate, 0);
      }
    } else {
    }
    break;

  case GLUT_KEY_RIGHT:
    if (shiftx - panRate > -374) {
      glTranslatef(-panRate, 0., 0.);
      shiftx += -panRate;
      // std::cout << shiftx << std::endl;
      PopupYes.changePosition(panRate, 0);
      PopupNo.changePosition(panRate, 0);
      for (int i = 0; i < numSpecialButtons; i++) {
        SpecialButtonList[i]->changePosition(panRate, 0);
      }
    } else {
    }
    break;
  case GLUT_KEY_UP:
    if (shifty + panRate > -421) {
      glTranslatef(0., panRate, 0.);
      shifty += -panRate;
      PopupYes.changePosition(0, -panRate);
      PopupNo.changePosition(0, -panRate);
      for (int i = 0; i < numSpecialButtons; i++) {
        SpecialButtonList[i]->changePosition(0, -panRate);
      }
    } else {
    }
    break;
  case GLUT_KEY_DOWN:
    if (shifty + panRate < 256) {
      glTranslatef(0., -panRate, 0.);
      shifty += panRate;
      // std::cout << shifty << std::endl;
      PopupYes.changePosition(0, panRate);
      PopupNo.changePosition(0, panRate);
      for (int i = 0; i < numSpecialButtons; i++) {
        SpecialButtonList[i]->changePosition(0, panRate);
      }
    } else {
    }
    break;
  }
  glutPostRedisplay();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  WIDTH = w;
  HEIGHT = h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0., WIDTH - 1, HEIGHT - 1, 0., -1.0, 1.0);
}

// the init function sets up the graphics card to draw properly
void init(void) {
  // clear the window to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // set up the coordinate system:  number of pixels along x and y
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0., WIDTH - 1, HEIGHT - 1, 0., -1.0, 1.0);

  // allow blending (for transparent textures)
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  // welcome message
  std::cout << "Welcome to Ole Compass." << std::endl;
}

void init_gl_window() {
  char *argv[] = {programName};
  int argc = sizeof(argv) / sizeof(argv[0]);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(programName);
  init();

  std::string texturePath = "StoMap.pam";
  campusmap = loadTexture(texturePath); // key to textures:  load them!

  glutDisplayFunc(drawWindow);
  glutReshapeFunc(reshape);
  // glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);
  glutMainLoop();
}

int main() {
  A.loadMapCalc("data/entireMapCalc.txt");
  A.equateNodes(NodeList);
  loadButtons("data/buttonList.txt");
  loadSpecialButtons("data/specialButtons.txt");
  B.loadMapCalcWeighted("data/entireMapCalc.txt", preferIndoors, avoidStairs,
                        false, false);
  init_gl_window();

  return 0;
}
