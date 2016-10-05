#include "Snake.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"


Snake::Snake() {

}

Snake::Snake(uniformData *CameraUBO, Renderer renderer) {
	this->snakeUnit = BatchUnit();
	pSystem = new ParticleSystem(30000, CameraUBO, glm::vec3(0, 0, 10));
	renderer.addToRenderer(pSystem->getRenderMode());

	snakePos = glm::vec3(-20 + +((float)rand() / (float)RAND_MAX) * 40.0f, 20.0f + ((float)rand() / (float)RAND_MAX) * 10.0f, -20 + +((float)rand() / (float)RAND_MAX) * 40.0f);
	snakeDir = glm::vec3(-1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f);
	snakeDirDeriv = glm::vec3(-1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f);
	snakeDirSecDeriv = glm::vec3(-1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + +((float)rand() / (float)RAND_MAX) * 2.0f);
	snakeUnit.transformationMatrix = glm::translate(snakeUnit.transformationMatrix, snakePos);
}


Snake::~Snake() {
}

void Snake::updateSnake(float timeDiff) {
	snakeDirSecDeriv += glm::vec3(-1.0 + ((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + ((float)rand() / (float)RAND_MAX) * 2.0f, -1.0 + ((float)rand() / (float)RAND_MAX) * 2.0f);
	snakeDirDeriv += snakeDirSecDeriv * timeDiff;
	snakeDir += snakeDirDeriv * timeDiff;
	snakeDir = glm::normalize(snakeDir);
	snakePos += glm::vec3(snakeDir.x * 5*  timeDiff, snakeDir.y * 5 * timeDiff, snakeDir.z * 5 * timeDiff);
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

BatchUnit *Snake::getUnit() {
	return &this->snakeUnit;
}