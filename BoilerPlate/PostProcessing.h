#pragma once

#include "Shader.h";
#include "Framebuffer.h"
#include <vector>
#include "Renderer.h"

#define SaturationAdjust 0
#define ContrastAdjust 1
#define BrightnessAdjust 2
#define GaussianBlur 3
#define BloomEffect 4
#define SpecularEffect 5


class PostProcessingModule {
public:
	
	PostProcessingModule() {
		
	}
	uint type;
	std::string getHeader() { return header; }
	std::string getMain() { return main; }
	std::string getBody() { return function; }
	std::vector<BoilerPlate::Shaders::uniform*> uniforms;

protected:
	const char* function;
	const void* constants;
	const char* shaderLocation;
	const char* header, *main;
	void getFunction() {
		std::string sFunc = BoilerPlate::Shaders::Shader::fileToString(shaderLocation);
		const std::string::size_type size = sFunc.size();
		char *buffer = new char[size + 1];   //we need extra char for NUL
		memcpy(buffer, sFunc.c_str(), size + 1);
		function = buffer;
	}
};


struct SaturationAdjustModule : public PostProcessingModule {
	struct SaturationData {
	public:
		float amount;
		SaturationData(float _amount) : amount(_amount) {};
	};


	SaturationAdjustModule(SaturationData *inData) {
		constants = (void*)&inData; 
		type = SaturationAdjust; 
		header = "uniform float SaturationAmount;\n";
		main = "SaturationEffect();\n";
		uniforms.push_back(new BoilerPlate::Shaders::uniform("SaturationAmount", &(inData->amount)));
		shaderLocation = "res/shaders/PostProcessingSaturationAdjustShader.glsl";
		getFunction();
	}
	
};

struct ContrastAdjustModule : public PostProcessingModule {
	struct ContrastData {
	public:
		float amount;
		ContrastData(float _amount) : amount(_amount) {};
	};
	ContrastAdjustModule(ContrastData *inData) { 
		constants = (void*)&inData; 
		type = ContrastAdjust;
		header = "uniform float ContrastAmount;\n";
		main = "ContrastEffect();\n";
		uniforms.push_back(new BoilerPlate::Shaders::uniform("ContrastAmount", &(inData->amount)));
		shaderLocation = "res/shaders/PostProcessingContrastAdjustShader.glsl";
		getFunction();
	}
};

struct BrightnessAdjustModule : public PostProcessingModule {
	struct BrightnessData {
	public:
		float amount;
		BrightnessData(float _amount) : amount(_amount) {};
	};

	
	BrightnessAdjustModule(BrightnessData *inData) { 
		constants = (void*)&inData; 
		type = BrightnessAdjust;
		header = "uniform float BrightnessAmount;\n";
		main = "BrightnessEffect();\n";
		uniforms.push_back(new BoilerPlate::Shaders::uniform("BrightnessAmount", &(inData->amount)));
		shaderLocation = "res/shaders/PostProcessingBrightnessAdjustShader.glsl";
		getFunction();
	}

};

struct GaussianBlurModule : public PostProcessingModule {
	struct GaussianBlurData {
	public:
		float amount, radius, passes;
		GaussianBlurData(float _am, float _ra, float _pa) : amount(_am), radius(_ra), passes(_pa) {}
	};


	GaussianBlurModule(GaussianBlurData *inData) { 
		constants = (void*)&inData; 
		type = GaussianBlur;
		header = "uniform float GaussianBlurAmount;\n uniform float GaussianBlurRadius;\n uniform float GaussianBlurPasses;\n";
		main = "GaussianBlurEffect();\n";
		shaderLocation = "res/shaders/GaussianBlurPostProcessing.glsl";
		getFunction();
	}


};

struct BloomEffectModule : public PostProcessingModule {
	struct BloomEffectData {
	public:
		float amount;
		int AttachmentID;
	};
	BloomEffectModule(BloomEffectData *inData) { 
		constants = (void*)inData; 
		type = BloomEffect;
		header = "uniform float BloomAmount;\n";
		main =  "BloomEffect();\n";
		uniforms.push_back(new BoilerPlate::Shaders::uniform("BloomAmount", &(inData->amount)));
		shaderLocation = "res/shaders/PostProcessingBloomShader.glsl";
		getFunction();
	}

};

struct SpecularEffectModule : public PostProcessingModule {
	struct SpecularEffectData {
	public:
		float amount;
		int AttachmentID;
	};
	const char* shaderLocation = "res/shaders/SpecularPostProcessing.glsl";
	SpecularEffectModule(SpecularEffectData inData) {
		constants = (void*)&inData;
		type = BloomEffect;
		header = "uniform float SpecularAmount;\n";
		main = "SpecularEffect();\n";

		function = BoilerPlate::Shaders::Shader::fileToString(shaderLocation).c_str();
	}

};


class PostProcessing {
public:
	struct PostProcessingRenderData {
		Framebuffer *outputBuffer, *inputBuffer;
		Renderer *renderer;
	};
	PostProcessing();
	PostProcessing(Framebuffer *FrameBufferIn);

	~PostProcessing();

	void RegisterEffect(uint Effect, void* data);
	void Render();

	bool generatePostProcess(Framebuffer *_inBuffer);

	inline Framebuffer* getOutputBuffer() { return outputBuffer; }
	static void PostProcessingRenderer(RenderMode *rm);

private:
	Entity* screenEntity;
	std::vector<PostProcessingModule*> modules;
	Framebuffer *outputBuffer, *inputBuffer;
	BoilerPlate::Shaders::Shader *PostProcessingShader;
	RenderMode *PostProcessingRenderMode;
	std::vector<Texture*> inputTextures;

	void setupOutputBuffer();

	float vertices[8] = {
		-1, -1,
		-1, 1,
		1, -1,
		1, 1
	};

	float textureCoords[8] = {
		0, 0,
		0, 1,
		1, 0,
		1, 1
	};

	unsigned int Indices[6] = {
		0, 1, 2,
		1, 2, 3
	};
	BoilerPlate::Properties::BufferObjectProperties floatPropsTwoD{
		2,
		sizeof(float),
		GL_FALSE,
		GL_ARRAY_BUFFER,
		GL_STATIC_DRAW,
		GL_FLOAT
	};

};


