#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Renderer.h"
#include "ParticleSystem.h"

class Snake {
public:
	Snake();
	Snake(uniformData *CameraUBO);
	~Snake();
	void updateSnake(float timeDiff);
	BatchUnit *getUnit();
	ParticleSystem * getParticleSystem();

private:
	glm::vec3 snakePos, snakeDir, snakeDirDeriv, snakeDirSecDeriv;
	BatchUnit snakeUnit;
	ParticleSystem *pSystem;
};

