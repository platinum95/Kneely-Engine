#include "PostProcessing.h"

#include <sstream>
#include "Constants.h"

PostProcessing::PostProcessing() {
}

PostProcessing::PostProcessing(Framebuffer *FrameBufferIn) {
	inputBuffer = FrameBufferIn;
}


PostProcessing::~PostProcessing() {
}


bool PostProcessing::generatePostProcess(Framebuffer *_inBuffer) {
	inputBuffer = _inBuffer;
	setupOutputBuffer();
	inputTextures = std::vector<Texture*>();
	std::stringstream header, main, body, together;
	std::string shader;
	header << "#version 330\n\n in vec2 texCoord;\n out vec4 outFrag;\n";
	main << "void main(){\n outFrag = texture(colour_attachment_0, texCoord);\n";

	screenEntity = new Entity();
	BufferObject* testVerts = new BufferObject(floatPropsTwoD, 0, 8, vertices);
	BufferObject* testTexs = new BufferObject(floatPropsTwoD, 1, 8, textureCoords);
	screenEntity->registerBufferObject(testVerts);
	screenEntity->registerBufferObject(testTexs);

	PostProcessingShader = new BoilerPlate::Shaders::Shader();
	PostProcessingShader->RegisterAttribute("position", 0);
	PostProcessingShader->RegisterAttribute("texCoord", 1);
	int i = 0;
	for (GLuint ca : inputBuffer->colour_attachments) {
		Texture *nextTexture = new Texture();
		inputTextures.push_back(nextTexture);
		nextTexture->texID = ca;
		nextTexture->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		nextTexture->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		nextTexture->target = GL_TEXTURE_2D;
		nextTexture->textureBank = GL_TEXTURE0 + i;
		screenEntity->textures.push_back(nextTexture);

		std::string variableName = std::string("colour_attachment_") + std::to_string(i);
		PostProcessingShader->RegisterTexture(variableName.c_str(), i);
		header << "uniform sampler2D " << variableName << ";";
		i++;
	}
	
	

	for (PostProcessingModule *ppm : modules) {
		header << ppm->getHeader();
		main << ppm->getMain();
		body << ppm->getBody();

		for (BoilerPlate::Shaders::uniform *uniform : ppm->uniforms) {
			PostProcessingShader->RegisterUniform(uniform);
			
		}
	}

	main << "}\n";

	together << header.str() << body.str() << main.str();
	shader = together.str();
	
	PostProcessingShader->addShaderType("res/shaders/PostProcessingVertexShader.glsl", GL_VERTEX_SHADER);
	PostProcessingShader->addShaderType(shader.c_str(), GL_FRAGMENT_SHADER, BoilerPlate::Shaders::StringType::CharArray);
	PostProcessingShader->LoadShader();

	for (PostProcessingModule *ppm : modules) {

		for (BoilerPlate::Shaders::uniform *uniform : ppm->uniforms) {
			screenEntity->uniforms.push_back(uniformData(uniform->uniformLocation,
				uniform->data, &BoilerPlate::Shaders::Shader::loadFloat2));
		}
	}

	
	
	screenEntity->createEntity(Indices, 6);
	screenEntity->units.push_back(new BatchUnit());
	PostProcessingRenderData *postProcessData = new PostProcessingRenderData;
	postProcessData->inputBuffer = inputBuffer;
	postProcessData->outputBuffer = outputBuffer;

	PostProcessingRenderMode = new RenderMode(GL_TRIANGLES, *PostProcessingShader);
	PostProcessingRenderMode->data = postProcessData;
	PostProcessingRenderMode->entityList.push_back(screenEntity);
	PostProcessingRenderMode->p = &PostProcessing::PostProcessingRenderer;

	return true;
}

void PostProcessing::setupOutputBuffer() {
	outputBuffer = new Framebuffer();
	outputBuffer->createBuffer(1);
	outputBuffer->genDepthBufferAttachment();
	outputBuffer->genDepthTextureAttachment();
	outputBuffer->genTextureAttachment();
	outputBuffer->unbindBuffer();

}

void PostProcessing::PostProcessingRenderer(RenderMode * rm) {
	PostProcessingRenderData* data = (PostProcessingRenderData*) rm->data;
	rm->shader.useShader();
	data->outputBuffer->bindBuffer();

	for (uniformData *ud : rm->shader.localUBOList) {
		if (ud->active)
			BoilerPlate::Shaders::Shader::loadUBOData(*ud);
	}
	for (Entity *ent : rm->entityList) {
		if (ent->active) {
			ent->bind();
			for (uniformData ud : ent->uniforms) {
				ud.p(ud.uniformLocation, ud.data);
			}
			for (BatchUnit *BU : ent->units) {
				if (BU->isActive()) {
					BoilerPlate::Shaders::Shader::loadMat42(ent->transformationLocation, &BU->transformationMatrix);
					glDrawElements(rm->mode, ent->indexCount, GL_UNSIGNED_INT, 0);
				}
			}
			ent->unbind();
		}
	}

	data->outputBuffer->unbindBuffer();
	rm->shader.stopShader();

}



void PostProcessing::RegisterEffect(uint Effect, void* data) {
	
	PostProcessingModule *newModule;
	switch (Effect) {
	case SaturationAdjust:
		newModule = new SaturationAdjustModule((SaturationAdjustModule::SaturationData*) data);
		break;
	case ContrastAdjust:
		newModule = new ContrastAdjustModule((ContrastAdjustModule::ContrastData*) data);
		break;
	case BrightnessAdjust:
		newModule = new BrightnessAdjustModule((BrightnessAdjustModule::BrightnessData*) data);
		break;
	case GaussianBlur:
		newModule = new GaussianBlurModule((GaussianBlurModule::GaussianBlurData*) data);
		break;
	case BloomEffect:
		newModule = new BloomEffectModule((BloomEffectModule::BloomEffectData*) data);
		break;
	case SpecularEffect:
		newModule = new SpecularEffectModule(*(SpecularEffectModule::SpecularEffectData*) data);
		break;

	}
	modules.push_back(newModule);

}

void PostProcessing::Render() {
	PostProcessingRenderMode->p(PostProcessingRenderMode);
}


