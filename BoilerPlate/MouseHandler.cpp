#include "MouseHandler.h"


bool *MouseHandler::button_state;
glm::vec2 MouseHandler::mouse_position;
std::vector<void(*)(int)> *MouseHandler::callback_list;

MouseHandler::MouseHandler() {
	unsigned char pixels[16 * 16 * 4];
	memset(pixels, 0xff, sizeof(pixels));
	GLFWimage image;
	image.width = 16;
	image.height = 16;
	image.pixels = pixels;
	GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);

	button_state = new bool[3];
	for (int i = 0; i < 3; i++)
		button_state[i] = false;
	callback_list = new std::vector<void(*)(int)>[3];
	for (int i = 0; i < 3; i++) {
		callback_list[i] = std::vector<void(*)(int)>();
	}
	glfwSetMouseButtonCallback(Constants::GLFWProps.window, onClickCallback);
	glfwSetCursorPosCallback(Constants::GLFWProps.window, onMoveCallback);
}


MouseHandler::~MouseHandler() {
	delete[] button_state;
	delete[] callback_list;
}

bool MouseHandler::isDown(uint button_number) {
	return button_state[button_number ];
}

glm::vec2 MouseHandler::getMousePosition() {
	return mouse_position;
}

void MouseHandler::addCallback(void(*_p)(int), uint button_number) {
	callback_list[button_number].push_back(_p);
}

void MouseHandler::onClickCallback(GLFWwindow* window, int button, int action, int mods) {
	button_state[button ] = action == GLFW_PRESS ? true : false;
	int size = callback_list[button ].size();
	if (size == 0) return;
	for (unsigned int i = 0; i < callback_list[button].size(); i++) {
		callback_list[button ].at(i)(action);
	}
}

void MouseHandler::onMoveCallback(GLFWwindow* window, double xpos, double ypos){
	mouse_position.x = (float) xpos;
	mouse_position.y = (float) ypos;
}

