
LDFLAGS= ../libsx86/BoilerPlate.a -lGL -lopenal -lGLEW 
INCLUDEDIR = -I/usr/include -I/usr/local/include -I../BoilerPlate -I./include -I../include
CC=g++
CFLAGS= -w -std=c++11 -g -O0 -Wall -Wextra -fpermissive
GLFWPKGCA = `pkg-config --cflags glfw3`
GLFWPKGCB = `pkg-config --static --libs glfw3`


ODIR=build
LDIR =../lib

LIBS=-lm

_DEPS = GL\GLEW.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Heartbeat.o Snake.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Specify the target file and the install directory
INSTALLDIR  = ../binaries

# Default target
.PHONY: all
all: OpenGL_Testing

$(ODIR)/%.o : src/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDEDIR)

#$(ODIR)/Heartbeat.o : src/Heartbeat.cpp
#	$(CC) -c -o $@ $< $(CFLAGS)
	
#$(ODIR)/Snake.o : src/Snake.cpp
#	$(CC) -c -o $(ODIR)/Snake.o Snake.cpp


OpenGL_Testing : $(OBJ) 
	$(CC) src/OpenGL_Testing.cpp $(GLFWPKGCA) -o $@ $^ $(LDFLAGS) $(INCLUDEDIR) $(CFLAGS) $(GLFWPKGCB)



	
clean:
	rm ./build/*.o
	rm ./OpenGL_Testing
	




