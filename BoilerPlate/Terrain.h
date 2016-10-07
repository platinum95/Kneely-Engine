#pragma once
#include "Entity.h"
#include "Shader.h"
#include "Renderer.h"

struct TerrainChunk{
	BufferObject *heightBO, *normalsBO;
	int xPos;
	int zPos;
	bool onScreen;
	bool loaded = true;
};

struct ChunkPosition{
	ChunkPosition(int _x, int _z) : x(_x), z(_z) {};
	int x, z;
};

class Terrain{
public:
	Terrain(float worldSize, unsigned int meshSize, const char* filepath, BoilerPlate::Shaders::Shader);
	~Terrain();
	void updateState(glm::vec3 worldPos);
	void updateCurrentUnits();
	void updateActiveChunks();
	float getHeight(glm::vec3 worldPos);
	glm::vec3 getNormal(int xPos, int zPos);
	void registerChunk(int xPos, int zPos);
	void generateChunk(int xPos, int zPos);
	void generateChunk(int xPos, int zPos, float mag1, float mag2, float mag3);
	void generateChunk(int xPos, int zPos, float from1, float from2, float from3, float to1, float to2, float to3);
	void loadVBO(BufferObject* BO);
	static void RenderTerrain(RenderMode*);
	RenderMode *TerrainRenderMode;
	void clearMeshData();

	Entity *chunkEntity;
private:
	uint meshSize;
	float worldSize;
	static const int MAX_CHUNKS;
	static int DATA_LENGTH;
	GLuint ivboID;
	void genHeightVBO(BufferObject* heightVBO);
	void genNormalVBO(BufferObject* normalVBO);
	int currentChunkPosx, currentChunkPosz;
	BatchUnit *currentUnits[9];
	void loadChunk(TerrainChunk*);
	void saveChunk(TerrainChunk*);
	void unloadChunk(TerrainChunk*);
	std::string getFilename(int x, int z);
	float* meshDataHolder;
	std::vector<BufferObject*> meshVBOHolder;
};

