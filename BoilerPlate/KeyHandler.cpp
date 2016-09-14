#include "KeyHandler.h"



KeyHandler::KeyHandler() {
	keyList = std::vector<keyType*>();
}


KeyHandler::~KeyHandler() {
}

void KeyHandler::addToList(keyType _in) {
	keyType *new_in = new keyType(_in);
	keyList.push_back(new_in);
}

void KeyHandler::addToList(keyType *_in) {
	keyList.push_back(_in);
}

void KeyHandler::update(GLFWwindow *window) {
	for (keyType *kt : keyList) {
		if (kt->handleType == KEY_INCREMENT) {
			if (kt->type == GLFW_CLICK) {
				if (!kt->isDownDown && glfwGetKey(window, kt->keyUP) == GLFW_PRESS) {
					kt->isDownDown = true;
					*(kt->toClock) += kt->byAmount;
				}
				if (kt->isDownDown && glfwGetKey(window, kt->keyUP) == GLFW_RELEASE) {
					kt->isDownDown = false;
				}
				if (!kt->isUpDown && glfwGetKey(window, kt->keyDOWN) == GLFW_PRESS) {
					kt->isUpDown = true;
					*(kt->toClock) -= kt->byAmount;
				}
				if (kt->isUpDown && glfwGetKey(window, kt->keyDOWN) == GLFW_RELEASE) {
					kt->isUpDown = false;
				}
			}
			else {
				if (glfwGetKey(window, kt->keyUP) == kt->type) {
					*(kt->toClock) += kt->byAmount;
				}
				if (glfwGetKey(window, kt->keyDOWN) == kt->type) {
					*(kt->toClock) -= kt->byAmount;
				}
			}
		}
		else if (kt->handleType == KEY_FUNCTION) {
			if (kt->type == GLFW_CLICK) {
				if (!kt->isDownDown && glfwGetKey(window, kt->keyUP) == GLFW_PRESS) {
					kt->isDownDown = true;
					kt->p(kt->param);
				}
				if (kt->isDownDown && glfwGetKey(window, kt->keyUP) == GLFW_RELEASE) {
					kt->isDownDown = false;
				}
			}
			else {
				if (glfwGetKey(window, kt->keyUP) == kt->type) {
					kt->p(kt->param);
				}
			}
		}
		else if (kt->type == KEY_IGNORE) {
			if (!kt->isDownDown && glfwGetKey(window, kt->keyUP) == GLFW_PRESS)
				kt->isDownDown = true;
			if (kt->isDownDown && glfwGetKey(window, kt->keyUP) == GLFW_RELEASE) {
				kt->isDownDown = false;
			}
			
		}
	}
}

void KeyHandler::cleanUp() {
	for (keyType *kt : keyList) {
		delete(kt);
	}
}
