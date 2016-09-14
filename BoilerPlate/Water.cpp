
#include "Water.h"
#include "Constants.h"
#define _USE_MATH_DEFINES
#include <math.h>

Water::Water(WaterProperties props) {
	cameraUBO = props.cameraUBO;
	lightingUBO = props.lightingUBO;

	reflectionFBO = new Framebuffer(Constants::DisplayProps.width, Constants::DisplayProps.height);
	refractionFBO = new Framebuffer(Constants::DisplayProps.width, Constants::DisplayProps.height);
	framebufferSetup();
	textureSetup();
	shaderSetup();
	entitySetup();
}

Water::Water(int width, int height, WaterProperties props) {
	cameraUBO = props.cameraUBO;
	lightingUBO = props.lightingUBO;

	reflectionFBO = new Framebuffer(width, height);
	refractionFBO = new Framebuffer(width, height);
	framebufferSetup();
	textureSetup();
	shaderSetup();
	entitySetup();

}

Water::Water(int ReflectionWidth, int ReflectionHeight, int RefractionWidth, int RefractionHeight, WaterProperties props) {
	cameraUBO = props.cameraUBO;
	lightingUBO = props.lightingUBO;

	reflectionFBO = new Framebuffer(ReflectionWidth, ReflectionHeight);
	refractionFBO = new Framebuffer(RefractionWidth, RefractionHeight);
	framebufferSetup();
	textureSetup();
	shaderSetup();
	entitySetup();

	
}

void Water::framebufferSetup() {

	reflectionFBO->createBuffer(1);
	reflectionFBO->genDepthBufferAttachment();
	reflectionFBO->genDepthTextureAttachment();
	reflectionFBO->genTextureAttachment();
	reflectionFBO->unbindBuffer();

	refractionFBO->createBuffer(1);
	refractionFBO->genDepthBufferAttachment();
	refractionFBO->genDepthTextureAttachment();
	refractionFBO->genTextureAttachment();
	refractionFBO->unbindBuffer();
}

void Water::entitySetup() {
	waterEntity = new Entity();
	BoilerPlate::Properties::BufferObjectProperties floatProps{
		3,
		sizeof(float),
		GL_FALSE,
		GL_ARRAY_BUFFER,
		GL_STATIC_DRAW,
		GL_FLOAT
	};
	BufferObject* waterVerts = new BufferObject(floatProps, 0, 12, waterPlaneVertices);
	waterEntity->registerBufferObject(waterVerts);
	waterEntity->createEntity(waterPlaneIndices, 6);
	waterEntity->uniforms.push_back(uniformData(waterShader.uniformTable.at(3)->uniformLocation,
		&waveOffset, &BoilerPlate::Shaders::Shader::loadFloat2));
	waterEntity->textures.push_back(reflectionTexture);
	waterEntity->textures.push_back(refractionTexture);
	waterEntity->textures.push_back(waterDUDV);
	waterEntity->textures.push_back(refractionDepthTex);
	waterEntity->textures.push_back(normalTexture);
	waterRenderMode = new RenderMode(GL_TRIANGLES, waterShader);
	waterRenderMode->entityList.push_back(waterEntity);
	waterRenderMode->p = &waterRenderer;
	waterEntity->units.push_back(new BatchUnit());

	waterPackage = new WaterPackage();
	waterPackage->reflectionFBO = reflectionFBO;
	waterPackage->refractionFBO = refractionFBO;
	waterPackage->waterEntity = waterEntity;

	waterRenderMode->data = (void*)waterPackage;

}

void Water::shaderSetup() {
	waterShader.RegisterAttribute("position", 0);
	waterShader.RegisterAttribute("texCoord", 1);
	waterShader.RegisterTexture("reflectionTex", 0);
	waterShader.RegisterTexture("refractionTex", 1);
	waterShader.RegisterTexture("DUDVMap", 3);
	waterShader.RegisterUniform("waveSet");
	waterShader.RegisterTexture("refractionDepth", 2);
	waterShader.RegisterTexture("normalMap", 4);
	waterShader.registerUBO(cameraUBO);
	waterShader.registerUBO(lightingUBO);

	waterShader.addShaderType("./res/shaders/waterVShader.glsl",
			GL_VERTEX_SHADER);
	waterShader.addShaderType("./res/shaders/waterFShader.glsl",
			GL_FRAGMENT_SHADER);
	waterShader.LoadShader();
	
}

void Water::textureSetup() {
	reflectionTexture = new Texture();
	reflectionTexture->texID = reflectionFBO->attachmentIDs[2];
	reflectionTexture->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	reflectionTexture->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	reflectionTexture->target = GL_TEXTURE_2D;
	reflectionTexture->textureBank = GL_TEXTURE0;


	refractionTexture = new Texture();
	refractionTexture->texID = refractionFBO->attachmentIDs[2];
	refractionTexture->target = GL_TEXTURE_2D;
	refractionTexture->textureBank = GL_TEXTURE1;

	refractionDepthTex = new Texture();
	refractionDepthTex->texID = refractionFBO->attachmentIDs[1];
	refractionDepthTex->target = GL_TEXTURE_2D;
	refractionDepthTex->textureBank = GL_TEXTURE2;

	ImageData waterData = ImageLoader::loadRAW("./res/images/waterDUDV.raw");
	waterDUDV = new Texture(waterData, GL_TEXTURE_2D);
	waterDUDV->registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	waterDUDV->registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	waterDUDV->textureBank = GL_TEXTURE3;
	waterDUDV->updateTexture();
	ImageLoader::freeData(waterData);

	ImageData normalData = ImageLoader::loadRAW("./res/images/water_normal.raw");
	normalTexture = new Texture(normalData, GL_TEXTURE_2D);
	normalTexture->registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	normalTexture->registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	normalTexture->textureBank = GL_TEXTURE4;
	normalTexture->updateTexture();
	ImageLoader::freeData(normalData);
}

Water::~Water() {
	reflectionTexture->deleteTexture();
	refractionTexture->deleteTexture();
	refractionDepthTex->deleteTexture();
	normalTexture->deleteTexture();
	waterDUDV->deleteTexture();
	
	reflectionFBO->cleanUp();
	refractionFBO->cleanUp();
	delete reflectionFBO, refractionFBO, waterRenderMode, refractionTexture, refractionDepthTex, reflectionTexture, waterDUDV, normalTexture;
}

void Water::setReflect(Camera &cam){

}

void Water::setRefract(Camera &cam) {

}

//Specific render procedure for water
void Water::waterRenderer(RenderMode *rm) {
	
	WaterPackage *wPackage = (WaterPackage*)rm->data;

	Camera backCam = *wPackage->camera;
	bool camAboveWater = wPackage->camera->position.y >= 0;
	wPackage->camera->position.y *= -1.0f;
	wPackage->camera->roll = (M_PI)- wPackage->camera->roll;
	wPackage->camera->updateCam();
	rm->deactivate();
	wPackage->clip_plane[3] = 0;

	wPackage->clip_plane[1] = wPackage->camera->position.y < 0 ? 1 : -1;
	wPackage->reflectionFBO->bindBuffer();
	wPackage->reflectionFBO->clear();
	wPackage->renderer->Render();
	wPackage->reflectionFBO->unbindBuffer();

	*wPackage->camera = backCam;
	wPackage->camera->updateCam();
	wPackage->clip_plane[1] *= -1;
	wPackage->refractionFBO->bindBuffer();
	wPackage->refractionFBO->clear();
	wPackage->renderer->Render();
	wPackage->refractionFBO->unbindBuffer();

	wPackage->clip_plane[3] = 1000;
	rm->activate();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Renderer::standardRender(rm);
	glDisable(GL_BLEND);
	
}

float Water::waterPlaneVertices[] = {
	-500, 0, 500,		//0
	-500, 0, -500,		//1
	500, 0, -500,		//2
	500, 0, 500			//3
};

uint Water::waterPlaneIndices[] = {
	0, 1, 3,
	1, 2, 3
};

char *Water::waterVShader = "";

char *Water::waterFShader = "";