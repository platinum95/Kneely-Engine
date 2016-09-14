#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Properties.h"
#include "Shader.h"
#include <vector>

namespace BoilerPlate {

	class BoilerPlate {
	public:
		BoilerPlate();
		~BoilerPlate();
		GLuint loadShader(const char * vPath, const char* fPath);
		Properties::GLFWproperties startGLFW(Properties::displayProperties);
		Properties::GLFWproperties getCurrentGLFWProperties();

		Properties::GLEWproperties startGLEW();

	private:
		Properties::GLFWproperties glfwProperties;
		Properties::GLEWproperties glewProperties;
		Shaders::Shader shaderList;

	};
}
