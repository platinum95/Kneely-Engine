#include "Renderer.h"
#include <GL/glew.h>
#include "Constants.h"

Renderer::Renderer() {
	toRender = std::vector<RenderMode*>();
}

Renderer::~Renderer() {
}

void Renderer::addToRenderer(RenderMode * toAdd) {
	this->toRender.push_back(toAdd);
}

void Renderer::Render() {
	for (uniformData *ud : BoilerPlate::Shaders::Shader::uboList) {
		if (ud->active)
			BoilerPlate::Shaders::Shader::loadUBOData(*ud);
	}
	for (RenderMode* rm : toRender) {
		if (rm->isActive()) {
			rm->p(rm);
		}
	}
}

void Renderer::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Constants::DisplayProps.width, Constants::DisplayProps.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(255, 255, 255, 1);	//Set the screen to white
}

void Renderer::standardRender(RenderMode * rm) {
	rm->shader.useShader();
	for (uniformData *ud : rm->shader.localUBOList) {
		if (ud->active)
			BoilerPlate::Shaders::Shader::loadUBOData(*ud);
	}
	for (Entity *ent : rm->entityList) {
		if (ent->active) {
			ent->bind();
			for (uniformData ud : ent->uniforms) {
				ud.p(ud.uniformLocation, ud.data);
			}
			for (BatchUnit *BU : ent->units) {
				if (BU->isActive()) {
					BoilerPlate::Shaders::Shader::loadMat42(ent->transformationLocation, &BU->transformationMatrix);
					glDrawElements(rm->mode, ent->indexCount, GL_UNSIGNED_INT, 0);
				}
			}
			ent->unbind();
		}
	}

	rm->shader.stopShader();
}

#pragma region RenderMode

RenderMode::RenderMode(GLenum _mode, BoilerPlate::Shaders::Shader _shader)
	: mode(_mode), shader(_shader) {
	active = true;
	p = Renderer::standardRender;
	data = NULL;
	
}
RenderMode::RenderMode() { 
	active = true; 
}

void RenderMode::activate() {
	active = true;
}

void RenderMode::deactivate() {
	active = false;
}

#pragma endregion
