#include "Snake.h"
#include "glm/gtc/quaternion.hpp"


Snake::Snake(uniformData *CameraUBO, Renderer renderer) {
	this->snakeUnit = BatchUnit();
	pSystem = ParticleSystem(30000, CameraUBO, glm::vec3(0, 0, 10));

}


Snake::~Snake() {
}

void Snake::updateSnake(float timeDiff) {
	snakeDirSecDeriv += glm::vec3(-1.0 + ((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + ((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + ((float)rand() / (float)RAND_MAX) * 2.0f);
	snakeDirDeriv += snakeDirSecDeriv * timeDiff;
	snakeDir += snakeDirDeriv * timeDiff;
	snakePos += glm::vec3(snakeDir.x * timeDiff, snakeDir.y * timeDiff, snakeDir.z * timeDiff);
	snakeUnit.transformationMatrix = glm::mat4();

	//		snakeUnit[i].transformationMatrix = glm::translate(snakeUnit->transformationMatrix, snakePos[i]);
	glm::vec3 EulerAngles(0, atan(snakeDir.x / snakeDir.x), atan(snakeDir.y / snakeDir.x));
	glm::quat MyQuaternion = glm::quat(EulerAngles);
	glm::mat4 rotateMat = glm::mat4_cast(MyQuaternion);

	snakeUnit.transformationMatrix = snakeUnit.transformationMatrix;
	snakeUnit.transformationMatrix = glm::translate(snakeUnit.transformationMatrix, snakePos)  * rotateMat;

	//	snakeUnit[i].transformationMatrix = rotationMat;
	//snakeUnit[i].transformationMatrix = glm::translate(snakeUnit->transformationMatrix, snakePos[i]);
}