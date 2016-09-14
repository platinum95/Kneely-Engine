/*
 * Framebuffer.h
 *
 *  Created on: Apr 19, 2016
 *      Author: peter
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <GL/glew.h>
#include <vector>
#include "Texture.h"


class Framebuffer {
	class FrameBufferStack {
	public:
		struct node {
		public:
			node* left, *right;
			Framebuffer* current;
		};
		node* head, *tail;
		unsigned int size;
		FrameBufferStack() { head = NULL; tail = NULL; size = 0; }
		void push_back(Framebuffer* in) {
			node* newNode = new node();
			newNode->current = in;
			newNode->left = tail;
			if (size == 0) {
				tail = newNode;
				head = newNode;
			}
			else {
				tail->right = newNode;
				tail = newNode;
			}
			size++;
			return;
		}
		Framebuffer* pop_back() {
			if (size == 0)
				return NULL;
			node* holder = tail->left;
			tail->left = tail->right = NULL;
			tail->current = NULL;
			delete tail;
			tail = holder;
			size--;
			if (size == 0)
				return NULL;
			return tail->current;

		}

	};
public:
	Framebuffer();
	Framebuffer(unsigned int width, unsigned int height);
	void RegisterBuffer();
	void createBuffer(unsigned int attachment_count);
	void bindBuffer();
	void unbindBuffer();
	static void unbindBufferNoStack(Framebuffer* in);
	std::vector<GLuint> attachmentIDs;
	void genDepthTextureAttachment();
	void genDepthBufferAttachment();
	void genTextureAttachment();
	void genTextureAttachment(Texture tex, GLuint colour_attachment);
	void cleanUp();
	void clear();
	void clearBlack();
	GLuint getID() { return id; }



	std::vector<GLuint> colour_attachments;
	static FrameBufferStack FramebufferStack;
	 
private:
	GLuint id;
	unsigned int width, height;
	bool bound;
	static void bindBufferNoStack(Framebuffer* buffer);

};

#endif /* FRAMEBUFFER_H_ */
