#include "TerrainGenerator.h"
#include <random>
#include <time.h>
#include <math.h>
#include <iostream>
#include <cstdlib>



unsigned int TerrainGenerator::seed = 0;

float MAX_HEIGHT = 40.0f;
float HEIGHT_SCALE = MAX_HEIGHT / (float) RAND_MAX;
float PI_O2 = 3.1415f / 2.0f;
TerrainGenerator::TerrainGenerator() {
	srand(time(NULL));
}


TerrainGenerator::~TerrainGenerator() {
}


float TerrainGenerator::getHeight(int x, int z) {
	float total = getInterpolatedNoise(x / 32.0f, z / 32.0f);
	total += getInterpolatedNoise(x/16.0f, z/16.0f)/2.0f;
	total += getInterpolatedNoise(x / 8.0f, z / 8.0f) / 7.0f;

	return total;
}

float TerrainGenerator::getHeight(int x, int z, float mag1, float mag2, float mag3) {
	float total = getInterpolatedNoise(x / 32.0f, z / 32.0f) / mag1;
	total += getInterpolatedNoise(x / 16.0f, z / 16.0f) / 2.0f / mag2;
	total += getInterpolatedNoise(x / 8.0f, z / 8.0f) / 7.0f / mag3;

	return total;
}

void TerrainGenerator::generateSeed() {
	srand(time(NULL));
	seed = 2483647;//(rand() % RAND_MAX);
	seed = ((float)seed / (float) RAND_MAX) * (4294967295.0f/2.0f);
}

float TerrainGenerator::getSmoothNoise(int x, int z) {
	float corners = (getNoise(x + 1, z + 1) + getNoise(x - 1, z + 1) + getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1))/16.0f;
	float sides = (getNoise(x + 1, z) + getNoise(x - 1, z) + getNoise(x, z - 1) + getNoise(x, z - 1)) / 8.0f;
	float centre = getNoise(x, z)/4.0f;
	return corners + centre + sides;
}

float TerrainGenerator::getNoise(int x, int z) {

	int n = x * 45 + z * 57;
	n = (n << 13) ^ n;
	int nn = (n*(n*n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	float toReturn = 1.0 - ((double)nn / 1073741824.0);
	return toReturn * MAX_HEIGHT;

}



float TerrainGenerator::interpolate(float a, float b, float blend) {
	float f = (1.0f - cos(blend * 3.145f)) * 0.5f;
	//float f = sin(blend * PI_O2);
	return a + (f)*(b-a);

}

float TerrainGenerator::getInterpolatedNoise(float x, float z) {
	int intX = (int)x;
	int intZ = (int)z;

	float fracX = x - intX;
	float fracZ = z - intZ;
	if (x <= 0) {
		fracX = 1 + fracX;
		intX--;
	}
	if (z <= 0) {
		fracZ = 1 + fracZ;
		intZ--;
	}
	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + 1, intZ);
	float v3 = getSmoothNoise(intX, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intZ + 1);
	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracZ);
}
