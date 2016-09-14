#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <vector>
#include "GL/glew.h"
#include "Properties.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>



typedef unsigned int uint;


struct uniformData {
	void* data;
	GLuint uniformLocation;
	uint binding_index, block_index;
	size_t dataSize;
	void(*p)(GLint, void*);
	uniformData(GLuint _loc, void* _data, void((*_p)(GLint, void*)))
		: uniformLocation(_loc), data(_data), p(_p) {
		type = 0;
	}
	uniformData(GLuint _type, void* _data, size_t _size, const char* _name)
		: type(_type), data(_data), dataSize(_size), name(_name) {
		
	}
	const char* name;
	bool active = true;
	GLuint type;
	uniformData() {}

};





namespace BoilerPlate {
	namespace Shaders{

		enum uniformType {
			Standard, Texture, UBO
		};
		enum StringType {
			File, CharArray
		};
		struct BufferAttribute {
			std::string attribName;
			int attribLocation;
			BufferAttribute(std::string _attribName, int _attribLocation) 
				: attribName(_attribName), attribLocation(_attribLocation) {}
		};

		struct uniform {
			std::string uniformName;
			GLuint uniformLocation;
			uniform(std::string _uniformName)
				: uniformName(_uniformName){
			}
			uniform(std::string _uniformName, void* _data)
				: uniformName(_uniformName), data(_data) {
			}
			uniformType type;
			GLint textureUnit;
			void *data;
		};

		struct shaderType {
			const char* filePath;
			uint type;
			StringType stringType;
		};

		class Shader {
		public:
			Shader();
			~Shader();
			Properties::ShaderProperties LoadShader();
			void addShaderType(shaderType* _st);
	
			shaderType* addShaderType(const char* _filePath, uint _type);
			shaderType* addShaderType(const char* _filePath, uint _type, StringType _stringType);
			static std::string fileToString(const char* path);
			Properties::ShaderProperties shaderProps;
			void useShader();
			void stopShader();
			void LoadSom(GLint loc, void* val);

			void RegisterAttribute(BufferAttribute*);
			BufferAttribute* RegisterAttribute(const char* _name, GLuint _location);
			void RegisterUniform(uniform*);
			uniform* RegisterUniform(const char* _name);
			uniform* RegisterTexture(const char* _name, GLint unit);
			uniform* RegisterUBO(const char* _name);
			void RegisterTexture(std::string, int);

			static void addUBO(uniformData* _ubo);
			static void generateUBO();
			void GetUniformLocations();
			void BindUBO();
			void registerUBO(uniformData* _ud);

			GLuint projLoc, viewLoc, timeLoc;
			std::vector<BufferAttribute*> attributeTable;
			std::vector<uniform*> uniformTable;
			std::vector<uniformData*> localUBOList;
			static std::vector<uniformData*> uboList;

			static void loadFloat2(GLint location, void* value);
			static void loadFloatArray(GLint location, void* value);
			static void loadFloatArray1544(GLint location, void * value);
			static void loadUBOData(uniformData _ud);
			static void loadVec32(GLint location, void* vector);
			static void loadVec42(GLint location, void* vector);
			static void loadBool2(GLint location, void* val);
			static void loadInt2(GLint location, void* val);
			static void loadMat42(GLint location, void* matrix);

		private:

			int attributeIndex;
			int uniLocation;
			std::vector<shaderType*> shaderList;
			
			std::vector<std::string> textureTable;

			void BindAttributes();

		};

	}
}
