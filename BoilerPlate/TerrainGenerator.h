#pragma once
#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H
class TerrainGenerator {
public:
	TerrainGenerator();
	~TerrainGenerator();
	static float getHeight(int x, int z);
	static float getHeight(int x, int z, float mag1, float mag2, float mag3);

	static void generateSeed();

	static unsigned int seed;

private:
	static float getSmoothNoise(int x, int z);
	static float getNoise(int x, int z);
	static float interpolate(float a, float b, float blend);
	static float getInterpolatedNoise(float x, float z);
};

#endif
