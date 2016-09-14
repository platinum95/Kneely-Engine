/*
 * Framebuffer.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: peter
 */

#include "Framebuffer.h"
#include "Constants.h"

Framebuffer::FrameBufferStack Framebuffer::FramebufferStack;
Framebuffer::Framebuffer(unsigned int _width, unsigned int _height) : width(_width), height(_height){
	attachmentIDs = std::vector<GLuint>();
	colour_attachments = std::vector<GLuint>();
	bound = false;
}
Framebuffer::Framebuffer() {
	width = Constants::DisplayProps.width;
	height = Constants::DisplayProps.height;
	attachmentIDs = std::vector<GLuint>();
	colour_attachments = std::vector<GLuint>();
	bound = false;
}



void Framebuffer::cleanUp(){
	glDeleteFramebuffers(1, &this->id);
}

void Framebuffer::createBuffer(uint attachment_count){
	glGenFramebuffers(1, &this->id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	
	if(attachment_count == 1)
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	if (attachment_count == 2) {
		GLenum myBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, myBuffers);
	}
	

}

void Framebuffer::bindBuffer(){
	if (bound)
		//return;
	bound = true;
	
	FramebufferStack.push_back(this);
	bindBufferNoStack(this);
}
void Framebuffer::bindBufferNoStack(Framebuffer* toBind) {
	glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, toBind->id);
	
	glViewport(0, 0, toBind->width, toBind->height);
}
void Framebuffer::unbindBuffer(){
	if (!bound)
	//	return;
	bound = false;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Constants::DisplayProps.width, Constants::DisplayProps.height);
	Framebuffer* nextBuffer = FramebufferStack.pop_back();
	if(nextBuffer != NULL)
		bindBufferNoStack(nextBuffer);

}

void Framebuffer::unbindBufferNoStack(Framebuffer* in) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Constants::DisplayProps.width, Constants::DisplayProps.height);
}

void Framebuffer::clear() {
	bool wasBound = true;
	if (!bound) {
		bindBuffer();
		wasBound = false;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(255, 255, 255, 1);	//Set the screen to white
	if (!wasBound) {
		unbindBuffer();
	}
	return;
}

void Framebuffer::clearBlack() {
	bool wasBound = true;
	if (!bound) {
		bindBuffer();
		wasBound = false;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);	//Set the screen to black
	if (!wasBound) {
		unbindBuffer();
	}
	return;
}

void Framebuffer::genTextureAttachment() {

	GLuint texture;
	glGenTextures(1, &texture);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
			0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colour_attachments.size(), texture, 0);
	attachmentIDs.push_back(texture);
	colour_attachments.push_back(texture);
	return;
}


void Framebuffer::genTextureAttachment(Texture tex, GLuint colour_attachment) {

	
	glFramebufferTexture(GL_FRAMEBUFFER, colour_attachment, tex.texID, 0);
	attachmentIDs.push_back(tex.texID);
	colour_attachments.push_back(colour_attachment);
	return;
}

void Framebuffer::genDepthTextureAttachment(){

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			texture, 0);
	attachmentIDs.push_back(texture);
	return;
}

void Framebuffer::genDepthBufferAttachment(){

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,
			height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuffer);
	attachmentIDs.push_back(depthBuffer);
	return;

}
