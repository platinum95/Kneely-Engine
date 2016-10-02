#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Renderer.h"


struct ParticlePack {
	Entity *particle_entity;
	glm::mat4 transformation_matrix;
	float time;
	uint particle_count;
};


class ParticleSystem {
public:
	ParticleSystem(uint Particle_Count, uniformData *cameraUBO);
	~ParticleSystem();

	
	void UpdateParticleSystem(float time_passed);

	static void RenderParticleSystem(RenderMode *rm);

	RenderMode* getRenderMode();

private:
	BoilerPlate::Shaders::Shader *particleShader;
	RenderMode *ParticleRenderMode;
	uniformData *time, *emitter_pos;
	float timeVal;
	void CreateShader(uniformData *cameraUBO);
	void CreateParticleSystem(uint particle_count);
	void SetupBuffers();
	const char* DEFAULT_FRAG_PATH = "res/shaders/ParticleFragmentDefaultShader.glsl";
	const char* DEFAULT_VERT_PATH = "res/shaders/ParticleVertexDefaultShader.glsl";
	BufferObject *initial_velocity_BO, *initial_time_BO;
	Entity *ParticleEntity;
	ParticlePack *pPack;

	BoilerPlate::Properties::BufferObjectProperties floatPropsThreeD{
		3,
		sizeof(float),
		GL_FALSE,
		GL_ARRAY_BUFFER,
		GL_STATIC_DRAW,
		GL_FLOAT
	};

	BoilerPlate::Properties::BufferObjectProperties floatPropsOneD{
		1,
		sizeof(float),
		GL_FALSE,
		GL_ARRAY_BUFFER,
		GL_STATIC_DRAW,
		GL_FLOAT
	};
};

#endif