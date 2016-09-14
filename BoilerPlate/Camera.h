#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct cameraData {
	float projection[16];
	float cam[16];
	float position[4];

};

class Camera {



public:


	Camera();
	~Camera();
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	glm::vec3 position;
	float pitch, yaw, roll;
	glm::mat4x4 viewMatrix;
	glm::mat4 cameraMatrix;
	glm::mat4 projection;

	cameraData camera_data;

	glm::mat4 updateCam();
	glm::vec4 direction;
	glm::vec4 plane;
	void moveZ(float amount);
	void moveX(float amount);
	void moveBy(glm::vec2);

	void updateCamData();

	Camera operator=(const Camera rhs);


};

#endif
