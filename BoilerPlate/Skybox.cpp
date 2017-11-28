#include "Skybox.h"
#include "ImageLoader.h"
#include <GL/glew.h>



GLuint Skybox::texID2;

Skybox::Skybox(BoilerPlate::Shaders::Shader skyboxShader) : Entity() {

	this->registerBufferObject(new BufferObject(Skybox::floatProps, 0, 24, vertices));
	this->createEntity(indices, 36);

	renderMode = RenderMode(GL_TRIANGLES, skyboxShader);
	this->units.push_back(new BatchUnit());
	renderMode.entityList.push_back(this);
}


Skybox::~Skybox() {
}

void Skybox::genTextures(std::string filepath[6]) {
	glGenTextures(1, &texID);
	Skybox::texID2 = texID;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	for (int i = 0; i < 6; i++) {
		ImageData data = ImageLoader::loadRAW(filepath[i].c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
		ImageLoader::freeData(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	skyboxTex.texID = texID;
	skyboxTex.textureBank = GL_TEXTURE0;
	skyboxTex.target = GL_TEXTURE_CUBE_MAP;
	this->textures.push_back(&skyboxTex);

}



#pragma region Constants

BoilerPlate::Properties::BufferObjectProperties Skybox::floatProps  {
	3,
	sizeof(float),
	GL_FALSE,
	GL_ARRAY_BUFFER,
	GL_STATIC_DRAW,
	GL_FLOAT
};

float Skybox::vertices[24] = {
	-500, 500, -500,		//0 T1
	-500, -500, -500,		//1 T1
	500, -500, -500,		//2 T2
	500, 500, -500,			//3 T1


	-500, 500, 500,			//4 T1
	-500, -500, 500,		//5 T1
	500, -500, 500,			//6 T2
	500, 500,  500			//7 T1

};

//GL_QUAD Clockwise
unsigned int Skybox::indices[36] = {
	0, 1, 3, 1, 2, 3,	//F1 Front
	2, 3, 7, 2, 7, 6,	//F2 Right
	4, 5, 7, 5, 7, 6,	//F3 Back
	0, 1, 4, 1, 4, 5,	//F4 Left
	1, 2, 6, 1, 6, 5,	//F5 Bottom
	0, 7, 4, 0, 7, 3	//F6 Top
};

#pragma endregion
