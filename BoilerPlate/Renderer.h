#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Entity.h"

struct RenderMode {
	GLuint VAOid;
	BoilerPlate::Shaders::Shader shader;
	std::vector<Entity*> entityList;
	GLenum mode;
	void(*p)(RenderMode*);
	void* data;

	RenderMode(GLenum _mode, BoilerPlate::Shaders::Shader _shader);
	RenderMode();

	void activate();
	void deactivate();
	inline bool isActive() {
		return active;
	}
private:
	bool active;

};

class Renderer {
public:
	Renderer();
	~Renderer();
	void addToRenderer(RenderMode*);
	void Render();
	void clear();

	static void standardRender(RenderMode*);
private:
	std::vector<RenderMode*> toRender;
	


};

#endif

