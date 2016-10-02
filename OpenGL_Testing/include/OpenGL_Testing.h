#pragma once
#include "BoilerPlate.h"
#include "Properties.h"
#include "Shader.h"
#include "Entity.h"
#include "ObjLoader.h"
#include "Vector.h"
#include "Matrix.h"
#include <cmath>
#include "ParticleSystem.h"

#include "Vector.h"
#include "Camera.h"
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <KeyHandler.h>
#include "Renderer.h"
#include <thread>
#include <iostream>
#include "Heartbeat.h"
#include <iomanip>
#include "Timer.h"
#include "AudioPlate.h"
#include "LoadAudio.h"
#include "AudioEntity.h"
#include "Skybox.h"
#include "BinaryLoader.h"
#include <sstream>
#include "TerrainGenerator.h"
#include "Terrain.h"
#include "PhysicsEntity.h"
#include "PhysicsConstants.h"
#include "PhysicsWorld.h"
#include "Scissor.h"
#include "Framebuffer.h"
#include "Constants.h"
#include "Water.h"
#include "MouseHandler.h"
#include "PostProcessing.h"
#include "Maths.h"
#include <glm\gtx\euler_angles.hpp>

#include "ModelLoader.h"

#include <direct.h>


unsigned int sphereVertexCount = 119406;
unsigned int sphereIndexCount = 238800;
unsigned int sphereDistanceCount = 39802;

float DisplayFrameTime;
float yOffset = 1.0f;
int DisplayFrame;

BoilerPlate::BoilerPlate boiler;
AudioPlate* audioPlate;

BoilerPlate::Properties::displayProperties displayProps = {
	1367,		//width
	768,		//height
	"Test",		//Title
	NULL,		//Monitor
	NULL,		//Shared context
	false		//Fullscreen
};

BoilerPlate::Properties::GLFWproperties glfwProps;
BoilerPlate::Properties::GLEWproperties glewProps;
BoilerPlate::Shaders::Shader sphereShader, groundShader2, normalShader, skyboxShader, entityShader, guiShader, waterShader;
BoilerPlate::Maths::vec3 eye(0, 0, 0);
BoilerPlate::Physics::DynamicEntity *player, *PhysicsCock;
Framebuffer *reflectionFBO, *refractionFBO;
float FOV = 90;
float Near_Plane = 0.1f;
float Far_Plane = 1000.0f;
float aspectRatio;

ParticleSystem *testPSystem;

double startingTime, changeFrameTime;

const char* planeVertexPath = "./res/shaders/sphereVShader.glsl", *planeFragmentPath = "./res/shaders/sphereFShader.glsl";
const char* groundVertexPath = "./res/shaders/groundVShader.glsl", *groundFragmentPath = "./res/shaders/groundFShader.glsl";
const char *normalVertexPath = "./res/shaders/normalVShader.glsl", *normalFragmentPath = "./res/shaders/normalFShader.glsl", *normalGeometryPath = "./res/shaders/normalGShader.glsl";
const char *skyboxVPath = "./res/shaders/skyboxVShader.glsl", *skyboxFPath = "./res/shaders/skyboxFShader.glsl";
const char *entityVPath = "./res/shaders/entityVShader.glsl", *entityFPath = "./res/shaders/entityFShader.glsl";
const char *guiVPath = "./res/shaders/guiVShader.glsl", *guiFPath = "./res/shaders/guiFShader.glsl";
const char *waterVPath = "./res/shaders/waterVShader.glsl", *waterFPath = "./res/shaders/waterFShader.glsl";
Entity *sphere, *ground, *scissors, *testTex, *tree;
BatchUnit *groundGroup, *scissorUnit, *treeUnit;
Skybox *skybox;
Texture *groundTex;
Camera camera;
bool keyDown[GLFW_KEY_Z - GLFW_KEY_A];
KeyHandler keyHandler;
keyType *forwardKey, *backKey, *leftKey, *rightKey, *spaceKey, *upCursor, *downCursor;
Terrain* terrain;
glm::vec3 cockPos(0, 0, 0);

MouseHandler *mouse;
void updateActiveEntities();

PostProcessing *postProcessPipeline;
WAV_File* testFile;
AudioEntity* music;
Timer *changeFrameTimer;
bool sendWave = false;
float waveAlpha = 0.0;
glm::mat4 translation[25];
glm::vec4 clipPlane = glm::vec4(0, -1, 0, 8);

Framebuffer *mainFrameBuffer, *toScreenBuffer;

int main();
void init();
void render();
void updateView();
void setupShaders();
void setupView();
void setupEntities();
void updateViewProps();
void checkMouse(int);
void cleanup();
static void changeWireframe(void*);
static void changeWireframe2(void*);
static void enableClip(void*);
static void disableSphere(void*);
static void forwardClick(void*);
static void forwardRelease(void*);
static void disableGround(void*);
bool forwardDown;
static bool groundOn = true;

static bool clipOn = false;
bool clippy = false;

glm::mat4 projection;
glm::mat4 cam;

static bool isHeld;
static glm::vec2 previousPos;
static float lastClock;
ContrastAdjustModule::ContrastData *cData;
BrightnessAdjustModule::BrightnessData *bData;
SaturationAdjustModule::SaturationData *sData;
BloomEffectModule::BloomEffectData *bloomData;

float updateInterval = 0.001;
float *timey = new float;
float timeSinceWave[3] = { 0, 0, 0 };
float timeDiff[3] = { 0, 0, 0 };

float vt[4] = { 0, 1, 2, 3 };



int phase = 0;
int clickedExit = 0;
int wireframe = 0, fillType = GL_LINE;



float waterPlaneTex[8] = {
	0, 1,
	0, 0,
	1, 1,
	1, 0
};


BoilerPlate::Properties::BufferObjectProperties floatProps  {
	3, 
	sizeof(float),
	GL_FALSE,
	GL_ARRAY_BUFFER,
	GL_STATIC_DRAW,
	GL_FLOAT
};
BoilerPlate::Properties::BufferObjectProperties floatPropsTwoD  {
	2,
	sizeof(float),
	GL_FALSE,
	GL_ARRAY_BUFFER,
	GL_STATIC_DRAW,
	GL_FLOAT
};







struct heartwave {
	float offsetwave[1544];
	float colourwave[1544 * 7];
} heart_wave;

struct light {
	float position[4];
	float colour[4];
} lighting;

struct clip {
	float plane[4] = { 0, 1, 0, 0 };
} clip_plane;

float testTexVert[8] = {
		-1, -1,
		-1, 1,
		1, -1,
		1, 1
};

float testTexTex[8] = {
		0, 0,
		0, 1,
		1, 0,
		1, 1
};

unsigned int testTexIndices[6] = {
		0, 1, 2,
		1, 2, 3
};

bool pauseRender = false;

Water *water;

uniformData testUniData;
uniformData* cameraUBO, *waveformUBO, *waveformUBO2, *lightingUBO, *clipUBO;
BufferObject *sphereVerticesBO;

WaterPackage *waterPackage;

uniformData timeUniform, projectionUniform, viewUniform, timeDiffUniform, waveformUniform, boxMVP;
RenderMode sphereRenderer, normalRenderer, skyboxRenderer, groundRenderer, scissorRenderer, testTexRenderer, waterRenderer, treeRenderer;

Renderer renderer, guiRenderer;

std::string skyboxFiles[] = { "./res/images/right.png", "./res/images/left.png", "./res/images/top.png", 
							  "./res/images/bottom.png", "./res/images/back.png", "./res/images/front.png" };

std::string skyboxFiles2[] = { "./res/images/right.raw", "./res/images/left.raw", "./res/images/top.raw",
							"./res/images/bottom.raw", "./res/images/back.raw", "./res/images/front.raw" };

std::string skyboxFiles3[] = { "./res/images/owen.raw", "./res/images/paula.raw", "./res/images/peter.raw",
"./res/images/frolov.raw", "./res/images/sully.raw", "./res/images/brian.raw" };
