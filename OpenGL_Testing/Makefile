
LDFLAGS= ../libsx86/BoilerPlate.a -lGL -lopenal -lGLEW 
INCLUDEDIR = -I/usr/include -I/usr/local/include -I../BoilerPlate -I./include
CC=g++
CFLAGS= -w -std=c++11 -g -O0 -Wall -Wextra
GLFWPKGCA = `pkg-config --cflags glfw3`
GLFWPKGCB = `pkg-config --static --libs glfw3`


ODIR=build
LDIR =../lib

LIBS=-lm

_DEPS = GL\GLEW.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Heartbeat.o Scissors.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Specify the target file and the install directory
INSTALLDIR  = ../binaries

# Default target
.PHONY: all
all: OpenGL_Testing

OpenGL_Testing : src/OpenGL_Testing.cpp $(OBJ)
	$(CC) $(GLFWPKGCA) -o $@ $< $(LDFLAGS) $(INCLUDEDIR) $(CFLAGS) $(GLFWPKGCB)


$(ODIR)/Heartbeat.o : src/Heartbeat.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(ODIR)/Scissors.o : src/Scissors.cpp
	$(CC) -c -o $@ $< $(INCLUDEDIR) $(CFLAGS)
	
clean:
	rm ./build/*.o
	rm ./OpenGL_Testing
	




