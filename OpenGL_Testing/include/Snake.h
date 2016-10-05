#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Renderer.h"
#include "ParticleSystem.h"

class Snake {
public:
	Snake(uniformData *CameraUBO, Renderer renderer);
	~Snake();
	void updateSnake(float timeDiff);
	BatchUnit *getUnit();

private:
	glm::vec3 snakePos, snakeDir, snakeDirDeriv, snakeDirSecDeriv;
	BatchUnit snakeUnit;
	ParticleSystem pSystem;
};

