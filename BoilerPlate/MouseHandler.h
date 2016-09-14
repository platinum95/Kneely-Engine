#pragma once

#include "Properties.h"
#include "Constants.h"
#include <glm\vec2.hpp>
#include <vector>

class MouseHandler {
public:
	MouseHandler();
	~MouseHandler();
	bool isDown(uint button_number);
	glm::vec2 getMousePosition();
	void addCallback(void(*_p)(int), uint buttonNumber);

private:
	static bool *button_state;
	static glm::vec2 mouse_position;
	static std::vector<void(*)(int)> *callback_list;
	static void onClickCallback(GLFWwindow* window, int button, int action, int mods);
	static void onMoveCallback(GLFWwindow* window, double xpos, double ypos);
};

