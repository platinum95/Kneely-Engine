#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLFW_CLICK 2
#define GLFW_HOLD 3
#define GLFW_CLICK_HOLD 4
#define KEY_INCREMENT 0
#define KEY_FUNCTION 1
#define KEY_IGNORE 0xFF

class keyType{
public:
	keyType(float *_toC, float _byAm, GLuint _keyUP, GLuint _keyDOWN, GLuint _type) : toClock(_toC), byAmount(_byAm), keyUP(_keyUP), keyDOWN(_keyDOWN), type(_type) {
		handleType = KEY_INCREMENT;
	};
	keyType(int *_toC, float _byAm, GLuint _keyUP, GLuint _keyDOWN, GLuint _type) : toClock((float*) _toC), byAmount(_byAm), keyUP(_keyUP), keyDOWN(_keyDOWN), type(_type) {
		handleType = KEY_INCREMENT;
	};
	keyType(void *_toC, void(*_p)(void*), GLuint _key, GLuint _type) : param((float*)_toC), p(_p), keyUP(_key), type(_type) {
		handleType = KEY_FUNCTION;
	};
	keyType(GLuint _key) : keyUP(_key) {
		type = KEY_IGNORE;
		handleType = KEY_IGNORE;
	};
	void(*p)(void*);
	float* toClock;
	void* param;
	float byAmount;
	GLuint keyUP, keyDOWN;
	bool isDownDown = false;
	bool isUpDown = false;
	GLuint type;
	unsigned int handleType;
};
class KeyHandler {
public:
	KeyHandler();
	~KeyHandler();
	void addToList(keyType);
	void addToList(keyType*);
	void update(GLFWwindow* window);
	
	std::vector<keyType*> keyList;
	void cleanUp();
};

