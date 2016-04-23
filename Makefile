CXX := g++
CXXFLAGS := -Wall -g -std=c++11 -Wno-deprecated-declarations
LIBS := -lGL -lglut -lm

ifneq ($(shell uname), Linux)
MACOSX_DEFINE := -I/sw/include
LIBS := -I/usr/common/include -I/usr/include/GL -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -framework OpenGL -lGL -lm -lobjc -lstdc++
endif


# here are the usual make rules

mapDraw: mapDraw.o texture.o Neighbor.o Point.o button.o Map.o Path.o
	$(CXX) $(CXXFLAGS) -o $(@) $(^) $(LIBS)


mapDraw.o: mapDraw.cpp
	$(CXX) $(CXXFLAGS) $(MACOSX_DEFINE) -c mapDraw.cpp

Path.o: Path.cpp Path.h Map.o
	$(CXX) $(CXXFLAGS) -c $(<)

Map.o: Map.cpp Map.h Neighbor.o
	$(CXX) $(CXXFLAGS) -c $(<)

Neighbor.o: Neighbor.cpp Neighbor.h Point.o
	$(CXX) $(CXXFLAGS) -c $(<)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $(<)


.PHONY: clean

clean:
	rm -f *.o mapDraw
