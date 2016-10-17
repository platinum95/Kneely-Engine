#include "Snake.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"

uint Snake::snake_count = 0;

Snake::Snake() {

}

Snake::Snake(uniformData *CameraUBO) {
	this->snakeUnit = BatchUnit();
	pSystem = new ParticleSystem(3000, CameraUBO, glm::vec3(0, 0, 10));


	snakePos = glm::vec3(-20 +((float)rand() / (float)RAND_MAX) * 40.0f, 20.0f + ((float)rand() / (float)RAND_MAX) * 10.0f, -20 + +((float)rand() / (float)RAND_MAX) * 40.0f);
	snakeDir = glm::vec3(-1.0 +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f);
	snakeDirDeriv = glm::vec3(-1.0 +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f);
	snakeDirSecDeriv = glm::vec3(-1.0 +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f);
	snakeUnit.transformationMatrix = glm::translate(snakeUnit.transformationMatrix, snakePos);

	snake_count++;
	id = snake_count;
}


Snake::~Snake() {
}

void Snake::updateSnake(float timeDiff, float shader_time) {
	snakeDirSecDeriv += glm::vec3(-1.0 + ((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + ((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + ((float)rand() / (float)RAND_MAX) * 2.0f) * 150.0f;
	snakeDirDeriv += snakeDirSecDeriv * timeDiff;
	snakeDir += snakeDirDeriv * timeDiff;
	snakeDir = glm::normalize(snakeDir);
	snakePos -= glm::vec3(snakeDir.x * 5*  timeDiff, snakeDir.y * 5 * timeDiff, snakeDir.z * 5 * timeDiff);
	snakeUnit.transformationMatrix = glm::mat4();

	glm::vec3 y(0.0, -snakeDir.z, snakeDir.y);
	glm::vec3 x = glm::cross(snakeDir, y);


	snakeUnit.transformationMatrix = glm::mat4(1.0f);
	snakeUnit.transformationMatrix[2] = glm::normalize(glm::vec4(snakeDir, 0));
	snakeUnit.transformationMatrix[1] = glm::normalize(glm::vec4(y, 0));
	snakeUnit.transformationMatrix[0] = glm::normalize(glm::vec4(x, 0));
	snakeUnit.transformationMatrix[3] = glm::vec4(snakePos, 1);

	pSystem->UpdateParticleSystem(timeDiff);
	glm::vec4 basePos = glm::vec4(0, 0, 4.8, 1); // (0, 0, 0, 1);
	glm::mat4 emitterTran = snakeUnit.transformationMatrix;
	float timey = fmod(shader_time + 3.14/2.0, 6.28);
	float sinVal = 0 - timey;
	float offset = 0.7 * sin(sinVal);
	float offsetC = 2.0 * cos(sinVal);
//	basePos.y += offset;
//	basePos.x += offsetC;
	glm::vec4 newEmitterPos = (emitterTran * basePos) ;
	pSystem->UpdateEmitterPos(newEmitterPos);
	pSystem->UpdateBaseDirection(-snakeDir);
	ParticlePack *pPack = pSystem->getParticlePack();
	pPack->transformation_matrix = snakeUnit.transformationMatrix * glm::translate(glm::vec3(basePos));
}

BatchUnit *Snake::getUnit() {
	return &this->snakeUnit;
}

ParticleSystem * Snake::getParticleSystem() {
	return this->pSystem;
}
