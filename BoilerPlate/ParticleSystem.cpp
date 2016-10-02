#include "ParticleSystem.h"



ParticleSystem::ParticleSystem(uint Particle_Count, uniformData *cameraUBO) {
	CreateShader(cameraUBO);
	CreateParticleSystem(Particle_Count);
}


ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::CreateParticleSystem(uint particle_count) {
	float *vv = new float[particle_count * 3]; // start velocities vec3
	float *vt = new float[particle_count]; // start times
	float t_accum;
	t_accum = 0.0f;
	int j = 0;
	srand(120444384);
	for (int i = 0; i < particle_count; i++) {
		// start times
		
		vt[i] = t_accum;
		t_accum += 0.01f;
		// start velocities. randomly vary x and z components
		float randx = ((float)rand() / (float)RAND_MAX) * 1.0f - 0.5f;
		float randz = ((float)rand() / (float)RAND_MAX) * 1.0f - 0.5f;
		float randy = ((float)rand() / (float)RAND_MAX) * 1.0f - 0.5f;
		vv[j] = randx; // x
		vv[j + 1] = randy; // y
		vv[j + 2] = randz; // z
		j += 3;
	}
	initial_velocity_BO = new BufferObject(floatPropsThreeD, 0, particle_count * 3, vv);
	initial_time_BO = new BufferObject(floatPropsOneD, 1, particle_count, vt);

	ParticleEntity = new Entity();
	ParticleEntity->registerBufferObject(initial_velocity_BO);
	ParticleEntity->registerBufferObject(initial_time_BO);
	ParticleEntity->createEntity();
	pPack = new ParticlePack;
	ParticleRenderMode = new RenderMode(GL_POINTS, *particleShader);
	ParticleRenderMode->p = &RenderParticleSystem;
	ParticleRenderMode->data = pPack;
	pPack->transformation_matrix = glm::translate(glm::vec3(0, 10, 0));
	pPack->particle_entity = ParticleEntity;
	pPack->particle_count = particle_count;
	timeVal = 0;
}

void ParticleSystem::UpdateParticleSystem(float time_passed) {
	if (timeVal == 0) {
		timeVal += time_passed;
		return;
	}
	timeVal += time_passed;
	pPack->time = timeVal;
}

void ParticleSystem::RenderParticleSystem(RenderMode * rm) {
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	ParticlePack *pp = (ParticlePack*) rm->data;
	
	rm->shader.useShader();
	pp->particle_entity->bind();
	for (uniformData *ud : rm->shader.localUBOList) {
		if (ud->active)
			BoilerPlate::Shaders::Shader::loadUBOData(*ud);
	}
	GLuint UBOLocation = rm->shader.uniformTable[0]->uniformLocation;
	BoilerPlate::Shaders::Shader::loadFloat2(UBOLocation, &pp->time);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	BoilerPlate::Shaders::Shader::loadMat42(pp->particle_entity->transformationLocation, (void*) &(pp->transformation_matrix));
	glDrawArrays(GL_POINTS, 0, pp->particle_count);
	pp->particle_entity->unbind();
	rm->shader.stopShader();

	glDisable(GL_BLEND);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

RenderMode * ParticleSystem::getRenderMode() {
	return this->ParticleRenderMode;
}

void ParticleSystem::CreateShader(uniformData *cameraUBO) {
	particleShader = new BoilerPlate::Shaders::Shader();
	particleShader->RegisterAttribute("init_velocity", 0);
	particleShader->RegisterAttribute("init_time", 1);
	particleShader->RegisterUniform("time");
	particleShader->RegisterUniform("emitter_position");
	particleShader->registerUBO(cameraUBO);
	particleShader->addShaderType(DEFAULT_VERT_PATH, GL_VERTEX_SHADER);
	particleShader->addShaderType(DEFAULT_FRAG_PATH, GL_FRAGMENT_SHADER);
	particleShader->LoadShader();
}

void ParticleSystem::SetupBuffers() {
}

