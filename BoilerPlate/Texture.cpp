#include "Texture.h"



Texture::Texture(ImageData data, GLenum type) {
	glGenTextures(1, &texID);
	this->target = type;
	active = true;
	this->data = new ImageData();
	*this->data = data;
}

Texture::~Texture() {
	delete this->data;
}
void Texture::deleteTexture() {
	glDeleteTextures(1, &texID);
}

Texture::Texture() {
	active = true;
}

void Texture::bind() {
	glBindTexture(this->target, texID);
}

void Texture::unbind() {
	glBindTexture(this->target, 0);
}

void Texture::registerParameter(GLenum _pname, GLint _param) {
	parameter toAdd{
		_pname,
		_param
	};
	this->parameters.push_back(toAdd);
}

void Texture::updateTexture() {
	glActiveTexture(textureBank);
	bind();
	glTexImage2D(this->target, 0, GL_RGBA, data->width, data->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->data);
	for (parameter p : parameters) {
		glTexParameteri(this->target, p.pname, p.param);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	unbind();
}
