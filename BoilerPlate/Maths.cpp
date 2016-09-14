#include "Maths.h"
#include <cmath>



float Maths::angleBetween(glm::vec2 one, glm::vec2 two) {
	glm::vec2 diff = one - two;
	bool xPos = diff.x > 0, yPos = diff.y > 0;

	float angle = atan(diff.y / diff.x);

	if (xPos && yPos)
		return angle;
	if (xPos && !yPos)
		return (2.0f * 3.1415f) + angle;
	if (!xPos && !yPos)
		return 3.1415f + angle;
	if (!xPos && yPos)
		return (3.1415f / 2.0f) - angle;

	return 0;
}

glm::vec3 Maths::angleBetween(glm::vec3 one, glm::vec3 two) {

	glm::vec3 toReturn(0, 0, 0);
	toReturn.x = angleBetween(glm::vec2(one.y, one.z), glm::vec2(two.y, two.z));
	toReturn.y = angleBetween(glm::vec2(one.x, one.z), glm::vec2(two.x, two.z));
	toReturn.z = angleBetween(glm::vec2(one.x, one.y), glm::vec2(two.x, two.y));

	return toReturn;
}
