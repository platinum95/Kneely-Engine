#include "Entity.h"

#define GLEW_STATIC


Entity::Entity() {
	VBOs = std::vector<BufferObject*>();
	textures = std::vector<Texture*>();
	units = std::vector<BatchUnit*>();
}

void Entity::createEntity(void* _index, int _iCount) {
	indices = _index;
	indexCount = _iCount;
	createVAO();
	glBindVertexArray(vaoID);
	storeVBOs();
	glBindVertexArray(0);
	storeVIO(indices, this->indexCount);
}


Entity::~Entity() {


}

void Entity::cleanUp() {
	for (BufferObject *bo : VBOs) {
		delete bo;
	}
}

int Entity::createVAO() {
	GLuint _vID;
	glGenVertexArrays(1, &_vID);
	this->vaoID = _vID;
	return vaoID;
}

void Entity::bind() {
	glBindVertexArray(vaoID);
	for (BufferObject* aVBO : VBOs) {
		if (aVBO->isActive())
			glEnableVertexAttribArray(aVBO->attrib);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVIO);
	for (Texture *tex : textures) {
		if (tex->isActive()) {
			glActiveTexture(tex->textureBank);
			glBindTexture(tex->target, tex->texID);
		}
	}
}

void Entity::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	for (BufferObject* aVBO : VBOs) {
		if (aVBO->isActive())
			glDisableVertexAttribArray(aVBO->attrib);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Entity::storeVBOs() {
	for (auto bo : VBOs) {
		glBindBuffer(bo->target, bo->vboID);
		if (bo->VBOtype == InstancedVBO) {
			glBindBuffer(bo->target, bo->vboID);
			InstancedBufferObject *ibo = (InstancedBufferObject*)bo;
			glBufferData(ibo->target, ibo->dataLength, ibo->data, GL_STREAM_DRAW);
			glVertexAttribPointer(ibo->attrib, ibo->dataLength, ibo->glType, false, ibo->stride, ibo->offset);
			glVertexAttribDivisor(bo->attrib, 1);
		}
		else{
			glGenBuffers(1, &bo->vboID);
			glBindBuffer(bo->target, bo->vboID);
			glBufferData(bo->target, bo->size * bo->unitSize, bo->data, bo->usage);
			glVertexAttribPointer(bo->attrib, bo->dimension, bo->glType, false, 0, 0);
			
		}
		glBindBuffer(bo->target, 0);
	}

}

GLuint Entity::createIVBO(uint maxSize) {
	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	
	glBufferData(GL_ARRAY_BUFFER, maxSize, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vboID;
}

BufferObject* Entity::getDistances(float vertices[], unsigned int vertexCount, GLuint attrib) {
	/*
	float *distances = new float[vertexCount/3];
	int j = 0;
	for (int i = 0; i < vertexCount; i += 3) {
		float x = vertices[i], y = vertices[i + 1];
		float dis = sqrt((x * x) + ((y - 10) * (y - 10)));
		if (dis < 100) {
			int hello = 20;
		}
		distances[j] = dis;
		j++;
	}
	BufferObject *newBO = new BufferObject();
	newBO->attrib = attrib;
	newBO->data = distances;
	newBO->dimension = 1;
	newBO->glType = GL_FLOAT;
	newBO->size = vertexCount / 3;
	newBO->target = GL_ARRAY_BUFFER;
	newBO->unitSize = sizeof(float);
	newBO->usage = GL_STATIC_DRAW;
	*/
	float *distances = new float[vertexCount / 3];
	int j = 0;
	for (unsigned int i = 0; i < vertexCount; i += 3) {
		float x = vertices[i], y = vertices[i + 1], z = vertices[i + 2];
		float xDiff = x - (-50);
		float yDiff = y - (0);
		float zDiff = z - (0);
		float dis = sqrt((xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff));

		distances[j] = dis;
		j++;
	}
	BufferObject *newBO = new BufferObject();
	newBO->attrib = attrib;
	newBO->data = distances;
	newBO->dimension = 1;
	newBO->glType = GL_FLOAT;
	newBO->size = vertexCount / 3;
	newBO->target = GL_ARRAY_BUFFER;
	newBO->unitSize = sizeof(float);
	newBO->usage = GL_STATIC_DRAW;

	return newBO;
}

BufferObject * Entity::getDistancesFromSpherePoint(float vertices[], unsigned int vertexCount, GLuint attrib, glm::vec3 P1, glm::vec3 origin, float radius) {
	float *distances = new float[vertexCount / 3];
	unsigned int distanceSize = vertexCount / 3;
	glm::vec3 vOP1 = (origin - P1);
	float OP1 = sqrt(pow(vOP1.x, 2) + pow(vOP1.y, 2) + pow(vOP1.z, 2));

	int j = 0;
	for (unsigned int i = 0; i < vertexCount; i += 3) {
		float x = vertices[i], y = vertices[i + 1], z = vertices[i + 2];
		glm::vec3 P2(x, y, z);
		glm::vec3 vP1P2 = P1 - P2;
		float P1P2 = (float) (pow(pow(vP1P2.x, 2) + pow(vP1P2.y, 2) + pow(vP1P2.z, 2), 0.5));

		glm::vec3 vOP2 = (origin - P2);
		float OP2 = sqrt(pow(vOP2.x, 2) + pow(vOP2.y, 2) + pow(vOP2.z, 2));

		float theta = acos(((OP2 * OP2) + (OP1 * OP1) - (P1P2 * P1P2))/(2 * OP1 * OP2));
		float dis = theta * radius;
		distances[j] = dis;
		j++;
	}
	
	std::ofstream out;
	out.open("./SphereDistace.cpp");
	out << "unsigned int sphereDistanceCount = " << vertexCount / 3 << ";\n";
	out << "float sphereDistances[] = {\n";
	for (unsigned int i = 0; i < distanceSize; i++) {
		char endChar = (i == vertexCount - 1) ? ' ' : ',';
		out << distances[i] << endChar << '\n';
	}
	out << "};\n";
	out.close();
	
	BufferObject *newBO = new BufferObject();
	newBO->attrib = attrib;
	newBO->data = distances;
	newBO->dimension = 1;
	newBO->glType = GL_FLOAT;
	newBO->size = vertexCount / 3;
	newBO->target = GL_ARRAY_BUFFER;
	newBO->unitSize = sizeof(float);
	newBO->usage = GL_STATIC_DRAW;

	return newBO;
}

void Entity::registerBufferObject(BufferObject* toAdd) {
	VBOs.push_back(toAdd);
}


void Entity::storeVIO(void * data, int size) {
	GLuint vioID;
	glGenBuffers(1, &vioID);
	indexVIO = vioID;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vioID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


#pragma region BufferObject
BufferObject::BufferObject(BoilerPlate::Properties::BufferObjectProperties boProp, GLuint attrib, unsigned int _size, void* _data)
	: attrib(attrib), dimension(boProp.dimension), size(_size), data(_data), target(boProp.target),
	usage(boProp.usage), glType(boProp.glType), unitSize(boProp.unitSize) {
}

void BufferObject::update(BoilerPlate::Properties::BufferObjectProperties boProp, GLuint attrib) {
	this->attrib = attrib;
	this->target = boProp.target;
	this->usage = boProp.usage;
	this->glType = boProp.glType;
	this->activate();
}

BufferObject::BufferObject() {
}

void BufferObject::activate() {
	active = true;
}
void BufferObject::deactivate() {
	active = false;
}

#pragma endregion


