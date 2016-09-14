#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <typeinfo>

namespace BoilerPlate {
	namespace Properties {
	
		struct displayProperties {
			int				width;
			int				height;
			const char*		title;
			GLFWmonitor*	monitor;
			GLFWwindow*		share;
			bool			fullscreen;
		};


		struct GLFWproperties {
			GLFWwindow*		window;
			GLFWmonitor*	monitor;
			bool			success;
		};

		struct GLEWproperties {
			GLenum	result;
			bool	success;
		};

		struct ShaderProperties {
			GLuint vertexID, fragmentID, geometryID, tesselationControlID, tesselationEvalID;
			GLuint programID;
			GLint vertexResult, fragmentResult, geometryResult, tesselationControlResult, tesselationEvalResult;
			GLint programResult;

		};

		struct BufferObjectProperties {
			unsigned int dimension;
			std::size_t unitSize;
			GLboolean transpose;
			GLenum target, usage, glType;
		};

		struct EntityProperties {
			float * vertices;
			unsigned int *indices;
			unsigned int vertexSize, indexSize;
			float *normals;
		};
	}
}

#endif
