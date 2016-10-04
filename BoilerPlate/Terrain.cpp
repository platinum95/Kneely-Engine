#include "Terrain.h"
#include "BinaryLoader.h"
#include <vector>
#include "TerrainGenerator.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()



const int Terrain::MAX_CHUNKS = 32;
int Terrain::DATA_LENGTH = 256 * 256 * 4;

Terrain::Terrain(float worldSize, unsigned int meshSize, const char* filepath, BoilerPlate::Shaders::Shader groundShader) {
	this->meshSize = meshSize;
	this->worldSize = worldSize;
	this->currentChunkPosx = 0;
	this->currentChunkPosz = 0;
	DATA_LENGTH = (meshSize * meshSize) * 4;
	chunkEntity = new Entity();
	std::vector<BufferObject*> meshVBOs = BinaryLoader::readFile(filepath);
	BufferObject *groundMeshFile = meshVBOs[1];
	BufferObject *groundMesh = new BufferObject();
	groundMesh->dimension = 2;
	uint dataCount = groundMeshFile->size / 3;
	uint groundMeshSize = (dataCount * 2);
	float* meshData = new float[groundMeshSize];
	float* meshFileData = (float*) groundMeshFile->data;
	for (unsigned int i = 0; i < dataCount; i++) {
		uint meshFileLoc = i * 3;
		uint meshLoc = i * 2;
		meshData[meshLoc] = meshFileData[meshFileLoc];
		meshData[meshLoc + 1] = meshFileData[meshFileLoc + 2];
	}

	groundMesh->attrib = 0;
	groundMesh->data = meshData;
	groundMesh->dimension = 2;
	groundMesh->glType = GL_FLOAT;
	groundMesh->unitSize = 4;
	groundMesh->size = (groundMeshFile->size * 2) / 3;
	groundMesh->target = GL_ARRAY_BUFFER;
	groundMesh->usage = GL_STATIC_DRAW;
	
	chunkEntity->registerBufferObject(groundMesh);

	for(int i = 0; i < 9; i++){
		currentUnits[i] = NULL;
	}

	TerrainRenderMode = new RenderMode(GL_TRIANGLES, groundShader);
	TerrainRenderMode->entityList.push_back(chunkEntity);
	TerrainRenderMode->p = &Terrain::RenderTerrain;

}


Terrain::~Terrain() {
}

void Terrain::generateChunk(int xPos, int zPos) {

	BatchUnit *newUnit = new BatchUnit();

	newUnit->extraData = new TerrainChunk();
	TerrainChunk *newChunkData = (TerrainChunk*) newUnit->extraData;
	newChunkData->xPos = xPos;
	newChunkData->zPos = zPos;
	newChunkData->heightBO = new BufferObject();
	newChunkData->normalsBO = new BufferObject();
	BufferObject* normalVBO = newChunkData->normalsBO;
	BufferObject* heightVBO = (BufferObject*) newChunkData->heightBO;
	heightVBO->data = new float[this->meshSize * this->meshSize];
	normalVBO->data = new float[this->meshSize * this->meshSize * 3];
	glm::vec3 worldPos(xPos * (this->worldSize * 1), 0, zPos * this->worldSize * 1);
	newUnit->transformationMatrix = glm::translate(worldPos);
	chunkEntity->units.push_back(newUnit);

	float *dataLoc = (float*)heightVBO->data;
	float *normLoc = (float*)normalVBO->data;

	for (unsigned int x = 0; x < this->meshSize; x++) {

		int xWPos = x + ((this->meshSize-1)*xPos);
		for (unsigned int z = 0; z < this->meshSize; z++) {
			int zWPos = z - ((this->meshSize-1) * (zPos));
			uint xIndex = z * this->meshSize;
			uint index = xIndex + x;
			dataLoc[index] = TerrainGenerator::getHeight(xWPos, zWPos);
			glm::vec3 norm = getNormal(xWPos, zWPos);
			normLoc[index * 3] = norm.x;
			normLoc[index * 3 + 1] = norm.y;
			normLoc[index * 3 + 2] = norm.z;

		}
	}
	genHeightVBO(heightVBO);
	genNormalVBO(normalVBO);
	saveChunk(newChunkData);

}

void Terrain::registerChunk(int xPos, int zPos) {

	BatchUnit *newUnit = new BatchUnit();

	newUnit->extraData = new TerrainChunk();
	TerrainChunk *newChunkData = (TerrainChunk*) newUnit->extraData;
	newChunkData->xPos = xPos;
	newChunkData->zPos = zPos;
	glm::vec3 worldPos(xPos * (this->worldSize * 1), 0, zPos * this->worldSize * 1);
	newUnit->transformationMatrix = glm::translate(worldPos);
	chunkEntity->units.push_back(newUnit);
	newChunkData->heightBO = new BufferObject();
	newChunkData->normalsBO = new BufferObject();
	genHeightVBO(newChunkData->heightBO);
	genNormalVBO(newChunkData->normalsBO);
	newChunkData->loaded = false;
}

void Terrain::generateChunk(int xPos, int zPos, float mag1, float mag2, float mag3) {
	BatchUnit *newUnit = new BatchUnit();
	newUnit->extraData = new TerrainChunk();
	TerrainChunk *newChunkData = (TerrainChunk*) newUnit->extraData;
	newChunkData->xPos = xPos;
	newChunkData->zPos = zPos;
	newChunkData->heightBO = new BufferObject();
	BufferObject* heightVBO = (BufferObject*) newChunkData->heightBO;
	heightVBO->data = new float[this->meshSize * this->meshSize];
	float *dataLoc = (float*)heightVBO->data;
	glm::vec3 worldPos(xPos * (this->worldSize * 1), 0, zPos * this->worldSize * 1);
	newUnit->transformationMatrix = glm::translate(worldPos);
	chunkEntity->units.push_back(newUnit);

	for (unsigned int x = 0; x < this->meshSize; x++) {

		int xWPos = x + ((this->meshSize - 1)*xPos);
		for (unsigned int z = 0; z < this->meshSize; z++) {
			int zWPos = z - ((this->meshSize - 1) * (zPos));
			uint xIndex = z * this->meshSize;
			uint index = xIndex + x;

			dataLoc[index] = TerrainGenerator::getHeight(xWPos, zWPos, mag1, mag2, mag3);

		}
	}
	genHeightVBO(heightVBO);

}

void Terrain::generateChunk(int xPos, int zPos, float from1, float from2, float from3, float to1, float to2, float to3) {
	BatchUnit *newUnit = new BatchUnit();
	newUnit->extraData = new TerrainChunk();
	TerrainChunk *newChunkData = (TerrainChunk*) newUnit->extraData;
	newChunkData->xPos = xPos;
	newChunkData->zPos = zPos;
	newChunkData->heightBO = new BufferObject();
	BufferObject* heightVBO = (BufferObject*) newChunkData->heightBO;
	heightVBO->data = new float[this->meshSize * this->meshSize];
	float *dataLoc = (float*)heightVBO->data;
	glm::vec3 worldPos(xPos * (this->worldSize * 1), 0, zPos * this->worldSize * 1);
	newUnit->transformationMatrix = glm::translate(worldPos);
	chunkEntity->units.push_back(newUnit);

	float diff1 = -(from1 - to1) / (this->meshSize - 1);
	float diff2 = -(from2 - to2) / (this->meshSize - 1);
	float diff3 = -(from3 - to3) / (this->meshSize - 1);
	for (unsigned int x = 0; x < this->meshSize; x++) {

		int xWPos = x + ((this->meshSize - 1)*xPos);
		float send1 = from1 + (diff1 * x);
		float send2 = from2 + (diff2 * x);
		float send3 = from3 + (diff3 * x);

		for (unsigned int z = 0; z < this->meshSize; z++) {
			int zWPos = z - ((this->meshSize - 1) * (zPos));
			uint xIndex = z * this->meshSize;
			uint index = xIndex + x;
			dataLoc[index] = TerrainGenerator::getHeight(xWPos, zWPos, send1, send2, send3);
		}
	}
	genHeightVBO(heightVBO);


}



void Terrain::RenderTerrain(RenderMode* rm) {
	rm->shader.useShader();
	for (uniformData *ud : rm->shader.localUBOList) {
		if (ud->active)
			BoilerPlate::Shaders::Shader::loadUBOData(*ud);
	}
	Entity* chunkEntity = rm->entityList[0];
	if (chunkEntity->active) {
		chunkEntity->bind();
		for (uniformData ud : chunkEntity->uniforms) {
			ud.p(ud.uniformLocation, ud.data);
		}
		for (BatchUnit *BU : chunkEntity->units) {
			if (BU->isActive()) {
				BoilerPlate::Shaders::Shader::loadMat42(chunkEntity->transformationLocation, &BU->transformationMatrix);

				BufferObject *heightVBO = ((TerrainChunk*)BU->extraData)->heightBO;
				BufferObject *normalVBO = ((TerrainChunk*)BU->extraData)->normalsBO;
				glBindBuffer(GL_ARRAY_BUFFER, heightVBO->vboID);
				glVertexAttribPointer(heightVBO->attrib, heightVBO->dimension, heightVBO->glType, false, 0, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, normalVBO->vboID);
				glVertexAttribPointer(normalVBO->attrib, normalVBO->dimension, normalVBO->glType, false, 0, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glEnableVertexAttribArray(heightVBO->attrib);
				glEnableVertexAttribArray(normalVBO->attrib);
				glDrawElements(rm->mode, chunkEntity->indexCount, GL_UNSIGNED_INT, 0);
				glDisableVertexAttribArray(heightVBO->attrib);
				glDisableVertexAttribArray(normalVBO->attrib);
			}
		}
		
		chunkEntity->unbind();
	}
	

	rm->shader.stopShader();
}


void Terrain::genHeightVBO(BufferObject * heightVBO) {

	glGenBuffers(1, &heightVBO->vboID);
	heightVBO->attrib = 1;
	heightVBO->unitSize = 4;
	heightVBO->dimension = 1;
	heightVBO->glType = GL_FLOAT;
	heightVBO->size = this->meshSize * this->meshSize;
	heightVBO->target = GL_ARRAY_BUFFER;
	heightVBO->usage = GL_DYNAMIC_DRAW;

	glBindBuffer(heightVBO->target, heightVBO->vboID);
	glBufferData(heightVBO->target, heightVBO->size * heightVBO->unitSize, NULL, heightVBO->usage);
	glVertexAttribPointer(heightVBO->attrib, heightVBO->dimension, heightVBO->glType, false, 0, 0);
	glBindBuffer(heightVBO->target, 0);
}

void Terrain::loadVBO(BufferObject* vbo){
	glBindBuffer(vbo->target, vbo->vboID);
	glBufferSubData(vbo->target, 0, vbo->size * vbo->unitSize, vbo->data);
	glBindBuffer(vbo->target, 0);
}
void Terrain::genNormalVBO(BufferObject * normalVBO) {
	glGenBuffers(1, &normalVBO->vboID);
	normalVBO->attrib = 3;
	normalVBO->unitSize = 4;
	normalVBO->dimension = 3;
	normalVBO->glType = GL_FLOAT;
	normalVBO->size = this->meshSize * this->meshSize * 3;
	normalVBO->target = GL_ARRAY_BUFFER;
	normalVBO->usage = GL_DYNAMIC_DRAW;

	glBindBuffer(normalVBO->target, normalVBO->vboID);
	glBufferData(normalVBO->target, normalVBO->size * normalVBO->unitSize, NULL, normalVBO->usage);
	glVertexAttribPointer(normalVBO->attrib, normalVBO->dimension, normalVBO->glType, false, 0, 0);
	glBindBuffer(normalVBO->target, 0);
}


glm::vec3 Terrain::getNormal(int xPos, int zPos){
	glm::vec3 worldPos(xPos, 0, zPos);
	float hL = TerrainGenerator::getHeight(xPos - 1, zPos);
	float hR = TerrainGenerator::getHeight(xPos + 1, zPos);
	float hD = TerrainGenerator::getHeight(xPos, zPos + 1);
	float hU = TerrainGenerator::getHeight(xPos, zPos - 1);
	glm::vec3 N(hL - hR, 2.0, hD - hU);
//	std::cout << "(" << N.x << ", " << N.y << ", " << N.z << ")\n";
	N = normalize(N);
	return N;
}

float Terrain::getHeight(glm::vec3 worldPos){
	glm::vec3 origiPos = worldPos;
	if(currentUnits[0] == NULL)
		return 0.0f;

	BatchUnit *BU = currentUnits[0];

	TerrainChunk* tc = (TerrainChunk*) BU->extraData;

	worldPos.x += this->worldSize/2.0f;
	worldPos.z += this->worldSize/2.0f;
	worldPos.x -= this->worldSize * tc->xPos;
	worldPos.z -= this->worldSize * tc->zPos;
	worldPos.z = this->meshSize - worldPos.z;
	int index = (int) (((worldPos.z > 0.0f ? floor(worldPos.z) : ceil(worldPos.z)) * this->meshSize) + (worldPos.x ? floor(worldPos.x) : ceil(worldPos.x)));
	float* hdata = (float*) tc->heightBO->data;
	float height = hdata[index];
	return height;

}


void Terrain::updateState(glm::vec3 worldPos){
//	std::cout << "(" << worldPos.x << ", " << worldPos.z << ")"  << std::endl;
	bool pos = worldPos.x > 0.0f;
	bool zpos = worldPos.z > 0.0f;
	worldPos.x *= pos ? 1.0f : -1.0f;
	worldPos.x += this->worldSize/2.0f;
	worldPos.z *= zpos ? 1.0f : -1.0f;
	worldPos.z += this->worldSize/2.0f;
	int xPos = (int) worldPos.x / (int) this->worldSize;
	int zPos = (int) worldPos.z / (int) this->worldSize;
	xPos *= pos ? 1 : -1;
	zPos *= zpos ? 1 : -1;
	if(xPos != currentChunkPosx || zPos != currentChunkPosz){
		currentChunkPosx = xPos;
		currentChunkPosz = zPos;
		updateActiveChunks();
		updateCurrentUnits();
//		std::cout << "(" << currentChunkPosx << ", " << currentChunkPosz << ")"  << std::endl;
	}

}

void Terrain::updateActiveChunks(){
	int midX = currentChunkPosx, midZ = currentChunkPosz;
	int aX = currentChunkPosx - 1, aZ = midZ - 1;
	int bX = currentChunkPosx	 , bZ = midZ - 1;
	int cX = currentChunkPosx + 1, cZ = midZ - 1;
	int dX = currentChunkPosx + 1, dZ = midZ	;
	int eX = currentChunkPosx + 1, eZ = midZ + 1;
	int fX = currentChunkPosx	 , fZ = midZ + 1;
	int gX = currentChunkPosx - 1, gZ = midZ + 1;
	int hX = currentChunkPosx - 1, hZ = midZ 	;

	BatchUnit *newUnits[9];
	for(int i = 0; i < 9; i++){
		newUnits[i] = NULL;
	}
	for (BatchUnit *BU : chunkEntity->units) {
		TerrainChunk *tc = (TerrainChunk*) BU->extraData;
		BU->active = false;
		if(tc->xPos == midX && tc->zPos == midZ)
			newUnits[0] = BU;
		else if(tc->xPos == aX && tc->zPos == aZ)
			newUnits[1] = BU;
		else if(tc->xPos == bX && tc->zPos == bZ)
			newUnits[2] = BU;
		else if(tc->xPos == cX && tc->zPos == cZ)
			newUnits[3] = BU;
		else if(tc->xPos == dX && tc->zPos == dZ)
			newUnits[4] = BU;
		else if(tc->xPos == eX && tc->zPos == eZ)
			newUnits[5] = BU;
		else if(tc->xPos == fX && tc->zPos == fZ)
			newUnits[6] = BU;
		else if(tc->xPos == gX && tc->zPos == gZ)
			newUnits[7] = BU;
		else if(tc->xPos == hX && tc->zPos == hZ)
			newUnits[8] = BU;
		else if(tc->loaded || tc->onScreen){
			unloadChunk(tc);
			tc->onScreen = false;
		}
	}
	for (int i = 0; i < 9; i++) {
		BatchUnit *oldBU = currentUnits[i];
		bool oldInNew = false;
		for(int j = 0; j < 9; j++){
			BatchUnit *newBU = newUnits[j];
			if(oldBU == newBU)
				oldInNew = true;
		}
		if(!oldInNew && oldBU != NULL){
			TerrainChunk *tc = (TerrainChunk*)oldBU->extraData;
			if(tc->loaded)
				unloadChunk(tc);
		}


	}
	for(int i = 0; i < 9; i++){
		currentUnits[i] = newUnits[i];
	}

}

void Terrain::updateCurrentUnits(){
	for(int i = 0; i < 9; i++){
		if(currentUnits[i] != NULL){
			TerrainChunk* tc = (TerrainChunk*) currentUnits[i]->extraData;
			if(!tc->loaded){
				loadChunk(tc);
				tc->loaded = true;
			}
			tc->onScreen = true;
			currentUnits[i]->active = true;
		}
	}
}

void Terrain::loadChunk(TerrainChunk* tc){
	std::string filename = getFilename(tc->xPos, tc->zPos);
	BinaryLoader loader;
	std::vector<BufferObject*> height = loader.readFile(filename.c_str());
	tc->heightBO->data = height[0]->data;
	tc->normalsBO->data = height[1]->data;
	loadVBO(tc->heightBO);
	loadVBO(tc->normalsBO);
	tc->loaded = true;

}
void Terrain::saveChunk(TerrainChunk* tc){
	std::string filename = getFilename(tc->xPos, tc->zPos);
	BinaryLoader loader;
	std::vector<BufferObject> bos = std::vector<BufferObject>();
	bos.push_back(*(tc->heightBO));
	bos.push_back(*(tc->normalsBO));
	loader.createFile(filename.c_str(), bos);

}

void Terrain::unloadChunk(TerrainChunk* tc){
	if(tc->loaded){
		delete tc->heightBO->data;
		delete tc->normalsBO->data;
	}
	tc->loaded = false;
}

std::string Terrain::getFilename(int x, int z){
	bool posX = x >= 0, posZ = z >= 0;
	int lXPos = abs(x), lZPos = abs(z);
	std::string xStr = (posX ? "P" : "N") + SSTR(lXPos);
	std::string zStr = (posZ ? "P" : "N") + SSTR(lZPos);
	return ("./temp/ground" + xStr + zStr + ".bin");

}


