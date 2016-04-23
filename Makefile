# this Makefile is intended to make any of the examples in this directory
#all:  proto-texture proto-ui proto-ui2 proto-counting proto-windows proto-sound proto-chat proto-multiplayer

# the following lines pertain to using sound
# if you download FMOD onto your own machine, change the line below
FMOD_DIR = /usr/local/dept/apps/fmod
FMOD_INCLUDE = -I$(FMOD_DIR)/api/inc -I$(FMOD_DIR)/examples/common
FMOD_LIB = $(FMOD_DIR)/api/lib/libfmodex64.so

# the following lines pertain to setting up compiler options and libraries
OPTS = -Wall -g -std=c++11
LIBS = -lGL -lglut -lm
ARCH := $(shell uname)
ifeq ($(ARCH), Linux)
else
 MACOSX_DEFINE = -DMACOSX -I/sw/include
 LIBS = -I/usr/common/include -I/usr/include/GL -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -framework OpenGL -lGL -lm -lobjc -lstdc++

endif

# here are the usual make rules

mapDraw: mapDraw.o texture.o Neighbor.o Point.o button.o Map.o Path.o
	g++ $(OPTS) -o mapDraw mapDraw.o texture.o Neighbor.o Point.o button.o Map.o Path.o $(LIBS)


mapDraw.o: mapDraw.cpp
	g++ $(OPTS) $(MACOSX_DEFINE) -c mapDraw.cpp

mapDraw2.o: mapDraw2.cpp
	g++ $(OPTS) $(MACOSX_DEFINE) -c mapDraw2.cpp

button.o: button.cpp
	g++ -g -Wall -std=c++11 -c button.cpp

texture.o: texture.cpp texture.h
	g++ $(OPTS) $(MACOSX_DEFINE) -c texture.cpp

Path.o: Path.cpp Path.h Map.o
	g++ -g -Wall -std=c++11 -c Path.cpp

Map.o: Map.cpp Map.h Neighbor.o
	g++ -g -Wall -std=c++11 -c Map.cpp

Neighbor.o: Neighbor.cpp Neighbor.h Point.o
	g++ -g -Wall -std=c++11 -c Neighbor.cpp

Point.o: Point.cpp Point.h
	g++ -g -Wall -std=c++11 -c Point.cpp

clean:
	rm *.o *~ proto-sound proto-texture proto-ui proto-ui2 proto-counting

