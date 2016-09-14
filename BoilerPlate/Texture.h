#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include "ImageLoader.h"
#include <GL/glew.h>
#include <vector>

struct parameter {
	GLenum pname;
	GLint param;
};

class Texture {
public:
	Texture(ImageData, GLenum type);
	~Texture();
	Texture();
	void bind();
	void unbind();
	void registerParameter(GLenum _pname, GLint _param);
	void updateTexture();
	void deleteTexture();
	GLenum textureBank;
	GLenum target;
	std::vector<parameter> parameters;
	GLuint texID;
	ImageData* data;
	inline bool isActive() {
		return active;
	}
private:
	bool active;
	
};

#endif
