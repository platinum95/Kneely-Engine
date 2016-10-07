#include "ParticleSystem.h"
#include <algorithm>
#include "glm/gtc/type_ptr.hpp"

ParticleSystem::ParticleSystem(uint Particle_Count, uniformData *cameraUBO) {
	CreateShader(cameraUBO);
	CreateParticleSystem(Particle_Count, glm::vec3(0, 1, 0));
}

ParticleSystem::ParticleSystem(uint Particle_Count, uniformData *cameraUBO, glm::vec3 base_direction) {
	CreateShader(cameraUBO);
	CreateParticleSystem(Particle_Count, base_direction);

}

ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::CreateParticleSystem(uint particle_count, glm::vec3 base_direction) {
	float *init_velocity_data = new float[particle_count * 3]; // start velocities vec3
	float *init_time_data = new float[particle_count]; // start times
	float *init_size_data = new float[particle_count]; // initial size
	float *init_colour_data = new float[particle_count * 3]; //initial colour
	float *init_opacity_data = new float[particle_count];	//initial opacity
	float *lifetime_data = new float[particle_count];	//lifetime
	//float *luminocity_data = new float[particle_count];	//luminocity

	float t_accum;
	t_accum = 0.0f;
	int j = 0;
	for (unsigned int i = 0; i < particle_count; i++) {
		// start times
		init_time_data[i] = t_accum;
		t_accum += 0.01f;

		//size
		float size = ((float)rand() / (float)RAND_MAX) * 3.0f;
		init_size_data[i] = size;

		//opacity
		float opacity = std::max(0.3f, (((float)rand() / (float)RAND_MAX)));
		init_opacity_data[i] = opacity;

		//lifetime
		float lifetime = std::max(1.0f, (((float)rand() / (float)RAND_MAX)) * 5.0f);
		lifetime_data[i] = lifetime;

		//colour
		float deltaG = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.2f;
		float deltaB = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.2f;
		init_colour_data[j] = 1.0f;	//r
		init_colour_data[j + 1] = 0.5498f + deltaG; // g
		init_colour_data[j + 2] = 0.27f + deltaB; // b


		// start velocity variance. randomly vary x, y and z components
		float randx = ((float)rand() / (float)RAND_MAX) * 1.0f - 0.5f;
		float randz = ((float)rand() / (float)RAND_MAX) * 1.0f - 0.5f;
		float randy = ((float)rand() / (float)RAND_MAX) * 1.0f - 0.5f;
		init_velocity_data[j] = randx * 1.0f; // x
		init_velocity_data[j + 1] = randy * 1.0f; // y
		init_velocity_data[j + 2] = randz * 1.0f; // z
		j += 3;
	}

	initial_velocity_BO = new BufferObject(floatPropsThreeD, 0, particle_count * 3, init_velocity_data);
	initial_time_BO = new BufferObject(floatPropsOneD, 1, particle_count, init_time_data);
	initial_size_BO = new BufferObject(floatPropsOneD, 2, particle_count, init_size_data);
	initial_colour_BO = new BufferObject(floatPropsThreeD, 3, particle_count * 3, init_colour_data);
	initial_opacity_BO = new BufferObject(floatPropsOneD, 4, particle_count, init_opacity_data);
	lifetime_BO = new BufferObject(floatPropsOneD, 5, particle_count, lifetime_data);

	ParticleEntity = new Entity();
	ParticleEntity->registerBufferObject(initial_velocity_BO);
	ParticleEntity->registerBufferObject(initial_time_BO);
	ParticleEntity->registerBufferObject(initial_size_BO);
	ParticleEntity->registerBufferObject(initial_colour_BO);
	ParticleEntity->registerBufferObject(initial_opacity_BO);
	ParticleEntity->registerBufferObject(lifetime_BO);
	ParticleEntity->createEntity();
	pPack = new ParticlePack;
	ParticleRenderMode = new RenderMode(GL_POINTS, *particleShader);
	ParticleRenderMode->p = &RenderParticleSystem;
	ParticleRenderMode->data = pPack;
	pPack->transformation_matrix = glm::translate(glm::vec3(0, 15, 0));
	pPack->emitter_pos = glm::vec4(0, 0, 0, 1);
	pPack->base_direction = base_direction;
	pPack->particle_entity = ParticleEntity;
	pPack->particle_count = particle_count;
	timeVal = 0;

	delete[] init_velocity_data; 
	delete[] init_time_data;
	delete[] init_size_data;
	delete[] init_colour_data;
	delete[] init_opacity_data; 
	delete[] lifetime_data;

	delete initial_velocity_BO;
	delete initial_time_BO;
	delete initial_size_BO;
	delete initial_colour_BO;
	delete initial_opacity_BO;
	delete lifetime_BO;

}

void ParticleSystem::UpdateParticleSystem(float time_passed) {
	if (timeVal == 0) {
		timeVal += time_passed;
		return;
	}
	timeVal += time_passed;
	pPack->time = timeVal;
}

void ParticleSystem::UpdateEmitterPos(glm::vec3 pos){
	pPack->emitter_pos = glm::vec4(pos, 1.0);
}

void ParticleSystem::MoveEmitterPos(glm::vec3 pos){
	pPack->emitter_pos += glm::vec4(pos, 0);
}

void ParticleSystem::UpdateBaseDirection(glm::vec3 direction) {
	pPack->base_direction = direction;
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

	BoilerPlate::Shaders::Shader::loadVec42(rm->shader.uniformTable[1]->uniformLocation, glm::value_ptr(pp->emitter_pos));
	BoilerPlate::Shaders::Shader::loadVec32(rm->shader.uniformTable[2]->uniformLocation, glm::value_ptr(pp->base_direction));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
	particleShader->RegisterAttribute("init_size", 2);
	particleShader->RegisterAttribute("init_colour", 3);
	particleShader->RegisterAttribute("init_opacity", 4);
	particleShader->RegisterAttribute("lifetime", 5);
	particleShader->RegisterUniform("time");
	particleShader->RegisterUniform("emitter_position");
	particleShader->RegisterUniform("base_direction");
	particleShader->registerUBO(cameraUBO);
	particleShader->addShaderType(DEFAULT_VERT_PATH, GL_VERTEX_SHADER);
	particleShader->addShaderType(DEFAULT_FRAG_PATH, GL_FRAGMENT_SHADER);
	particleShader->LoadShader();
}

void ParticleSystem::SetupBuffers() {
}

