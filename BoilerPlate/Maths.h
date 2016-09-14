#pragma once

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

class Maths {
public:
	static float angleBetween(glm::vec2 one, glm::vec2 two);
	static glm::vec3 angleBetween(glm::vec3 one, glm::vec3 two);
};

