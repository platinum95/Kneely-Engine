#pragma once

#ifndef WATER_H
#define WATER_H

#include "Camera.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "Entity.h"
#include "Shader.h"

struct WaterPackage {
	Renderer *renderer;
	Camera *camera;
	float *clip_plane;
	Entity *waterEntity;
	Framebuffer *reflectionFBO, *refractionFBO;
};

struct WaterProperties {
	uniformData *cameraUBO, *lightingUBO;
};
class Water {
public:
	Water(WaterProperties props);
	Water(int width, int height, WaterProperties props);
	Water(int ReflectionWidth, int ReflectionHeight, int RefractionWidth, int RefractionHeight, WaterProperties props);
	~Water();

	void setReflect(Camera &cam);
	void setRefract(Camera &cam);
	static void waterRenderer(RenderMode *rm);

	RenderMode *waterRenderMode;
	float waveOffset = 0.0f;
	uniformData *cameraUBO, *lightingUBO;
	WaterPackage *waterPackage;
	Texture *groundTex, *reflectionTexture, *refractionTexture, *waterDUDV, *refractionDepthTex, *normalTexture;

private:
	Entity *waterEntity;
	Framebuffer *reflectionFBO, *refractionFBO;
	void framebufferSetup();
	void entitySetup();
	void textureSetup();
	void shaderSetup();

	
	BoilerPlate::Shaders::Shader waterShader;

	static float waterPlaneVertices[];
	static uint waterPlaneIndices[];
	static char *waterVShader, *waterFShader;

};

#endif