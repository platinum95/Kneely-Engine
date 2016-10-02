#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include "GL/glew.h"
#include "Properties.h"
#include <typeinfo>
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include <glm/gtx/transform.hpp>

#define standardVBO 1
#define InstancedVBO 2

struct BufferObject {
	GLuint attrib, vboID;
	void* data;
	unsigned int dimension, size;
	std::size_t unitSize;
	GLenum target, usage, glType;
	BufferObject(BoilerPlate::Properties::BufferObjectProperties boProp, GLuint attrib, unsigned int _size, void* _data);
	void update(BoilerPlate::Properties::BufferObjectProperties boProp, GLuint attrib);
	BufferObject();
	void activate();
	void deactivate();
	int VBOtype;
	inline bool isActive() {return active;}
	const char* title;
	
private:
	bool active = true;
};

struct InstancedBufferObject : BufferObject {
	InstancedBufferObject() { VBOtype = InstancedVBO; }
	uint max_size;
	uint stride;
	void* offset;
	uint dataLength;
};

class BatchUnit {
public:
	bool active = true;
	glm::mat4 transformationMatrix;
	void* extraData;
	inline bool isActive() { return active; }
};
class Entity {
public:
	Entity();
	~Entity();
	void createEntity(void* indices, int);
	void createEntity();
	void cleanUp();
	int createVAO();
	void bind();
	void unbind();
	void storeVBOs();
	GLuint createIVBO(uint maxSize);

	void storeVIO(void*, int size);
	static BufferObject* getDistances(float vertices[], unsigned int vertexCount, GLuint attrib);
	static BufferObject * getDistancesFromSpherePoint(float vertices[], unsigned int vertexCount, GLuint attrib, glm::vec3 point, glm::vec3 origin, float radius);


	//float *vertices, *distances;
	void *indices;
	int indexCount;
	GLuint vaoID;
	GLuint indexVIO;
	void registerBufferObject(BufferObject*);
	GLuint transformationLocation;

	std::vector<BufferObject*> VBOs;
	std::vector<uniformData> uniforms;
	std::vector<Texture*> textures;
	std::vector<BatchUnit*> units;
	bool active = true;


};

#endif


