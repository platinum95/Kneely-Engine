#pragma once
#include "Entity.h"
#include "Renderer.h"
#include <string>
#include "Shader.h"

class Skybox : Entity {
public:
	Skybox(BoilerPlate::Shaders::Shader skyboxShader);
	~Skybox();
	void genTextures(std::string filepath[6]);
	RenderMode renderMode;

	static void skyboxRenderer(RenderMode*);
	Texture skyboxTex;
private:
	GLuint texID;
	static GLuint texID2;
	static float vertices[24];
	static unsigned int indices[36];
	static BoilerPlate::Properties::BufferObjectProperties floatProps;
};

