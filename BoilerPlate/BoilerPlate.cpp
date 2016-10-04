

#include "BoilerPlate.h"
#include "Constants.h"


namespace BoilerPlate {

	BoilerPlate::BoilerPlate() {
	}


	BoilerPlate::~BoilerPlate() {
	}


	GLuint BoilerPlate::loadShader(const char * vPath, const char* fPath) {
		shaderList = Shaders::Shader();
		shaderList.addShaderType(vPath, GL_VERTEX_SHADER);
		shaderList.addShaderType(fPath, GL_FRAGMENT_SHADER);
		shaderList.LoadShader();
		return shaderList.shaderProps.programID;
	}


	Properties::GLFWproperties BoilerPlate::startGLFW(Properties::displayProperties props) {
		int initSuccess = glfwInit();

		if (props.monitor == NULL)
			glfwProperties.monitor = props.fullscreen ? glfwGetPrimaryMonitor() : NULL;
		else
			glfwProperties.monitor = props.monitor;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	  	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwProperties.window = glfwCreateWindow(props.width, props.height, props.title, glfwProperties.monitor, props.share); // Windowed
		glfwProperties.success = (glfwProperties.window) && initSuccess ? 1 : 0;
		glfwMakeContextCurrent(glfwProperties.window);

		Constants::DisplayProps = props;
		Constants::GLFWProps = glfwProperties;
		return glfwProperties;
	}

	Properties::GLFWproperties BoilerPlate::getCurrentGLFWProperties() {
		return this->glfwProperties;
	}

	Properties::GLEWproperties BoilerPlate::startGLEW() {
		//glewExperimental = GL_TRUE;
		glewProperties.result = glewInit();
		glewProperties.success = (glewProperties.result == GLEW_OK) ? true : false;
		glEnable(GL_DEPTH_TEST);

		return glewProperties;
	}

}
