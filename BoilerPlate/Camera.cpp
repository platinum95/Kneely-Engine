#include "Camera.h"





Camera::Camera() {
	position = glm::vec3(0, 0, 0);
	direction = glm::vec4(0, 0, 1, 0);
	plane = glm::vec4(0, 0, 1, 0);
}


Camera::~Camera() {
}


void Camera::rotateX(float angle) {
	glm::mat4 rotate = glm::mat4();
	rotate = glm::rotate(angle, glm::vec3(position.x, 0, 0));
	glm::vec4 res(position, 1);
	res = rotate * res;
	position.x = res.x;
	position.y = res.y;
	position.z = res.z;
}

void Camera::rotateY(float angle) {
	glm::mat4 rotate = glm::mat4();
	rotate = glm::rotate(angle, glm::vec3(0, position.y, 0));
	glm::vec4 res(position, 1);
	res = rotate * res;
	position.x = res.x;
	position.y = res.y;
	position.z = res.z;
}


void Camera::rotateZ(float angle) {
	glm::mat4 rotate = glm::mat4();
	rotate = glm::rotate(angle, glm::vec3(0, 0, position.z));
	glm::vec4 res(position, 1);
	res = rotate * res;
	position.x = res.x;
	position.y = res.y;
	position.z = res.z;
}

glm::mat4 Camera::updateCam() {
	glm::vec3 pos = glm::vec3(this->position.x, this->position.y, this->position.z);
	glm::mat4 toTran = glm::translate(this->position);
	direction = glm::vec4(sin(this->roll) * sin(this->yaw), cos(this->roll), sin(this->roll) * cos(this->yaw), 1);
	plane = glm::vec4(direction.x, direction.y, direction.z, glm::distance(pos, glm::vec3(0, 0, 0)));
	glm::vec4 toLook = toTran * direction;
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 lookeHere(toLook.x, toLook.y, toLook.z);
	cameraMatrix = glm::lookAt(pos, lookeHere, up);

	updateCamData();

	return this->cameraMatrix;
}

void Camera::moveZ(float amount) {
	glm::vec4 toMove(direction.x * amount, direction.y *amount, direction.z * amount, 0);
	this->position += glm::vec3(toMove);
}

void Camera::moveX(float amount) {
	glm::vec3 rDir = glm::vec3(direction);
	rDir = glm::rotateY(rDir, glm::pi<float>() * 0.5f);
	glm::vec4 toMove(rDir.x * amount, rDir.y *amount, rDir.z * amount, 0);
	this->position += glm::vec3(toMove.x, 0, toMove.z);
}

void Camera::moveBy(glm::vec2 byVec) {
	this->yaw -= byVec.x;
	this->roll += byVec.y;
}

void Camera::updateCamData() {
	for (int i = 0; i < 16; i++) {
		int index1 = i / 4;
		int index2 = i % 4;
		camera_data.cam[i] = cameraMatrix[index1][index2];
		camera_data.projection[i] = projection[index1][index2];
	}
	camera_data.position[0] = position.x;
	camera_data.position[1] = position.y;
	camera_data.position[2] = position.z;
	camera_data.position[3] = 1.0;


}

Camera Camera::operator=(const Camera rhs) {
	this->position = glm::vec3(rhs.position);
	this->pitch = rhs.pitch;
	this->yaw = rhs.yaw;
	this->roll = rhs.roll;
	this->viewMatrix = glm::mat4x4(rhs.viewMatrix);
	this->cameraMatrix = glm::mat4(rhs.cameraMatrix);
	this->projection = glm::mat4(rhs.projection);
	this->direction = glm::vec4(rhs.direction);
	this->plane = glm::vec4(rhs.plane);
	return *this;
}


