IDIR = /usr/include
LOCAL_INCLUDE = ../include
CC=g++
CFLAGS=-I$(IDIR) -I$(LOCAL_INCLUDE) -O0 -w -std=c++11 -fpermissive -pg -D_DEBUG -g -c -Wall

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = GL\GLEW.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Properties.o  ImageLoader.o TerrainGenerator.o Constants.o Terrain.o Timer.o KeyHandler.o AudioEntity.o Skybox.o AudioPlate.o LoadAudio.o BoilerPlate.o Camera.o Texture.o Entity.o PhysicsWorld.o PhysicsEntity.o PhysicsConstants.o BinaryLoader.o ObjLoader.o Renderer.o lodepng.o Water.o Framebuffer.o Shader.o ModelLoader.o MouseHandler.o ParticleSystem.o PostProcessing.o Timing.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Specify the target file and the install directory
OUTPUTFILE  = BoilerPlate.a
INSTALLDIR  = ../libsx86

# Default target
.PHONY: all
all: $(OUTPUTFILE)

$(ODIR)/%.o : %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/Skybox.o : Skybox.cpp $(ODIR)/Entity.o $(ODIR)/ImageLoader.o $(ODIR)/Renderer.o $(ODIR)/Shader.o
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/Terrain.o : Terrain.cpp $(ODIR)/Entity.o $(ODIR)/BinaryLoader.o $(ODIR)/Renderer.o $(ODIR)/Shader.o $(ODIR)/TerrainGenerator.o
	$(CC) -c -o $@ $< $(CFLAGS)


$(ODIR)/Renderer.o : Renderer.cpp $(ODIR)/Entity.o
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/Texture.o : Texture.cpp $(ODIR)/ImageLoader.o
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/Entity.o : Entity.cpp $(ODIR)/Texture.o
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/ObjLoader.o : ObjLoader.cpp $(ODIR)/Entity.o $(ODIR)/Properties.o
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/BinaryLoader.o : BinaryLoader.cpp $(ODIR)/Entity.o $(ODIR)/ObjLoader.o
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/BoilerPlate.o : BoilerPlate.cpp $(ODIR)/Shader.o $(ODIR)/Properties.o $(ODIR)/Constants.o
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/Shader.o : Shader.cpp $(ODIR)/Properties.o
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/ImageLoader.o : ImageLoader.cpp $(ODIR)/lodepng.o
	$(CC) -c -o $(ODIR)/ImageLoader.o ImageLoader.cpp

$(ODIR)/AudioPlate.o : AudioPlate.cpp $(ODIR)/LoadAudio.o
	$(CC) -c -o $(ODIR)/AudioPlate.o AudioPlate.cpp $(CFLAGS)
	
$(ODIR)/PhysicsConstants.o : PhysicsConstants.cpp 
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(ODIR)/PhysicsEntity.o : PhysicsEntity.cpp 
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/ModelLoader.o : ModelLoader.cpp 
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(ODIR)/Framebuffer.o : Framebuffer.cpp $(ODIR)/Constants.o
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/Constants.o : Constants.cpp $(ODIR)/Properties.o
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(ODIR)/PhysicsWorld.o : PhysicsWorld.cpp $(ODIR)/PhysicsEntity.o $(ODIR)/PhysicsConstants.o
	$(CC) -c -o $@ $< $(CFLAGS)


$(ODIR)/lodepng.o : lodepng.cpp
	$(CC) -c -o $(ODIR)/lodepng.o lodepng.cpp

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

BoilerPlate: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

$(OUTPUTFILE): $(OBJ) 
	ar ru $@ $^
	ranlib $@
	cp $(OUTPUTFILE) $(INSTALLDIR)/$(OUTPUTFILE)

.PHONY: install
install:
	cp $(OUTPUTFILE) /usr/lib
	cp ./*.h /usr/include/BP

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
