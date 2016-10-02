#include "Shader.h"



using namespace std;
namespace BoilerPlate {
	namespace Shaders {
		
		std::vector<uniformData*> Shader::uboList = std::vector<uniformData*>();

		Shader::Shader() {
			shaderList = std::vector<shaderType*>();
			localUBOList = std::vector<uniformData*>();
		}

		Shader::~Shader() {
			;
		}

		Properties::ShaderProperties Shader::LoadShader() {
			//TODO: add error checking

			this->shaderProps.programID = glCreateProgram();

			for (shaderType *shaderP : shaderList) {
				shaderType shader = *shaderP;

				const char *csrc = " ";
				std::string ssrc;
				if (shaderP->stringType == StringType::File) {
					ssrc = fileToString(shader.filePath);
					csrc = ssrc.c_str();
				}
				else if (shaderP->stringType == StringType::CharArray) {
					csrc = shader.filePath;
				}
				

				GLuint id;
				switch (shader.type) {
				case GL_VERTEX_SHADER:
					this->shaderProps.vertexID = glCreateShader(GL_VERTEX_SHADER);
					id = this->shaderProps.vertexID;
					break;
				case GL_FRAGMENT_SHADER:
					this->shaderProps.fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
					id = this->shaderProps.fragmentID;
					break;
				case GL_GEOMETRY_SHADER:
					this->shaderProps.geometryID = glCreateShader(GL_GEOMETRY_SHADER);
					id = this->shaderProps.geometryID;
					break;
				case GL_TESS_CONTROL_SHADER:
					this->shaderProps.tesselationControlID = glCreateShader(GL_TESS_CONTROL_SHADER);
					id = this->shaderProps.tesselationControlID;
					break;
				case GL_TESS_EVALUATION_SHADER:
					this->shaderProps.tesselationEvalID = glCreateShader(GL_TESS_EVALUATION_SHADER);
					id = this->shaderProps.tesselationEvalID;
					break;
				}
				glShaderSource(id, 1, &csrc, NULL);
				glCompileShader(id);

				GLint success = 1;
				glGetShaderiv(id, GL_COMPILE_STATUS, &success);
				if (!success) {
					char buffer[512];
					glGetShaderInfoLog(id, 512, NULL, buffer);
					std::cout << shader.filePath << " " << buffer << std::endl;
				}
				else {
					glAttachShader(this->shaderProps.programID, id);
				}

			}

			this->BindAttributes();
			glLinkProgram(this->shaderProps.programID);
			GLint progStatus;
			glGetProgramiv(this->shaderProps.programID, GL_LINK_STATUS, &progStatus);
			if (progStatus == GL_FALSE) {
				char buffer[512];
				glGetProgramInfoLog(this->shaderProps.programID, 512, NULL, buffer);
				std::cout << buffer << std::endl;
			}
			GLint success;
			success = glGetError();
			glValidateProgram(this->shaderProps.programID);
			glUseProgram(this->shaderProps.programID);
			success = glGetError();
			this->GetUniformLocations();
			this->BindUBO();
			stopShader();
			return this->shaderProps;
		}

		

		void Shader::addShaderType(shaderType * _st) {
			this->shaderList.push_back(_st);
		}


		shaderType * Shader::addShaderType(const char * _filePath, uint _type) {
			shaderType* newType = new shaderType();
			newType->filePath = _filePath;
			newType->type = _type;
			newType->stringType = StringType::File;
			addShaderType(newType);
			return newType;

		}
		shaderType * Shader::addShaderType(const char* _cArray, uint _type, StringType _stringType){
			shaderType* newType = new shaderType();
			newType->filePath = _cArray;
			newType->type = _type;
			newType->stringType = StringType::CharArray;
			addShaderType(newType);
			return newType;

		}

		std::string Shader::fileToString(const char* path) {
			ifstream inStream(path);
			if (inStream.fail()) {
				cout << "Error opening file";
				return "ERR";
			}
			string shaderString, currentLine = "";

			while (!inStream.eof()) {
				getline(inStream, currentLine);
				shaderString.append(currentLine + '\n');
			}

			inStream.close();
			return shaderString;
		}
		/*
		renderObject * Shader::registerObject(GLuint VBO_location) {
			renderObject * newRO = new renderObject(VBO_location);
			this->registerObject(newRO);

		}

		void Shader::registerObject(renderObject toAdd) {
			this->objectsToRender.push_back(toAdd);
		}
		*/
		void Shader::useShader() {
			glUseProgram(this->shaderProps.programID);
		}

		void Shader::stopShader() {
			glUseProgram(0);
		}
		void Shader::loadFloat2(GLint location, void* value) {
			float* val = (float*)value;
			glUniform1f(location, *val);
		}

		void Shader::loadFloatArray(GLint location, void * value) {
			float* val = (float*)value;
			glUniform1fv(location, 3, val);
		}
		void Shader::loadFloatArray1544(GLint location, void * value) {
			float* val = (float*)value;
			glUniform1fv(location, 1544, val);
		}

		void Shader::loadUBOData(uniformData _ud) {

			glBindBuffer(GL_UNIFORM_BUFFER, _ud.uniformLocation);
			glBufferData(GL_UNIFORM_BUFFER, _ud.dataSize, _ud.data, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void Shader::LoadSom(GLint loc, void* val) {
			float* data = (float*)val;

		}


		void Shader::loadVec32(GLint location, void* vector) {
			glm::vec3* val = (glm::vec3*) vector;
			glUniform3f(location, val->x, val->y, val->z);
		}

		void Shader::loadVec42(GLint location, void* vector) {
			glm::vec4* val = (glm::vec4*) vector;
			glUniform4f(location, val->x, val->y, val->z, val->w);
		}

		void Shader::loadBool2(GLint location, void * val) {
			int ival = *(bool*)val ? 1 : 0;
			glUniform1i(location, ival);
		}

		void Shader::loadInt2(GLint location, void * val) {
			int ival = *(int*)val;
			glUniform1i(location, ival);
		}

		void Shader::loadMat42(GLint location, void* matrix) {
			glm::mat4x4* mat = (glm::mat4x4*)matrix;
			glm::mat4x4 mat2 = *mat;
			glUniformMatrix4fvARB(location, 1, GL_FALSE, &mat2[0][0]);
		}


		void Shader::RegisterTexture(std::string s, int i) {
			this->textureTable.push_back(s);
		}

		void Shader::RegisterAttribute(BufferAttribute* attrib) {
			this->attributeTable.push_back(attrib);
		}

		BufferAttribute* Shader::RegisterAttribute(const char * _name, GLuint _location) {
			BufferAttribute *newAttrib = new BufferAttribute(_name, _location);
			this->RegisterAttribute(newAttrib);
			return newAttrib;
		}

		void Shader::RegisterUniform(uniform* uniform) {
			this->uniformTable.push_back(uniform);
		}

		uniform * Shader::RegisterUniform(const char * _name) {
			uniform *newUniform = new uniform(_name);
			newUniform->type = uniformType::Standard;
			this->RegisterUniform(newUniform);
			return newUniform;
		}

		uniform * Shader::RegisterTexture(const char * _name, GLint unit) {
			uniform *newUniform = new uniform(_name);
			newUniform->type = uniformType::Texture;
			newUniform->textureUnit = unit;
			this->RegisterUniform(newUniform);
			return newUniform;
		}
		uniform * Shader::RegisterUBO(const char * _name) {
			uniform *newUniform = new uniform(_name);
			newUniform->type = uniformType::UBO;
			this->RegisterUniform(newUniform);
			return newUniform;
		}


		void Shader::addUBO(uniformData * _ubo) {
			uboList.push_back(_ubo);
		}

		void Shader::generateUBO() {
			for (uniformData *ubo : uboList) {
				GLuint uboID = 0;
				glGenBuffers(1, &uboID);
				ubo->uniformLocation = uboID;
				glBindBuffer(GL_UNIFORM_BUFFER, ubo->uniformLocation);
				glBufferData(GL_UNIFORM_BUFFER, ubo->dataSize, ubo->data, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
		}

		void Shader::GetUniformLocations() {
			for (auto u = uniformTable.begin(); u != uniformTable.end(); ++u) {
				uniform* current = *u;
				current->uniformLocation = glGetUniformLocation(shaderProps.programID, current->uniformName.c_str());
				if (current->type == uniformType::Texture) {
					int value = current->textureUnit;
					Shader::loadInt2(current->uniformLocation, &value);
				}
			}
		}

		void Shader::BindUBO() {
			for (uniformData* ud : localUBOList) {
				ud->block_index = glGetUniformBlockIndex(this->shaderProps.programID, ud->name);
				glBindBufferBase(GL_UNIFORM_BUFFER, ud->binding_index, ud->uniformLocation);
				glUniformBlockBinding(this->shaderProps.programID, ud->block_index, ud->binding_index);
			}
		}

		void Shader::registerUBO(uniformData * _ud) {
			this->localUBOList.push_back(_ud);
		}

		void Shader::BindAttributes() {
			for (auto BA = attributeTable.begin(); BA != attributeTable.end(); ++BA) {
				BufferAttribute* current = *BA;
				glBindAttribLocation(this->shaderProps.programID, current->attribLocation, current->attribName.c_str());
			}
		}


	}




}
