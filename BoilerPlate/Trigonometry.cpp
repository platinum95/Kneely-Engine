#include "Trigonometry.h"


float Trigonometry::degreesToRadian(float degrees) {
	return (float) ((degrees * M_PI) / 180);
}

float Trigonometry::radianToDegrees(float radians) {
	return (float)(radians * (180 / M_PI));
}

float Trigonometry::ctan(float t) {
	return (float)1 / tan(t);
}
