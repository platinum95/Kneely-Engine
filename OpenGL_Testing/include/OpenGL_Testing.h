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
#include <glm/gtc/quaternion.hpp>
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
#include "Framebuffer.h"
#include "Constants.h"
#include "Water.h"
#include "MouseHandler.h"
#include "PostProcessing.h"
#include <glm/gtx/euler_angles.hpp>
#include "Strings.h"
#include "ModelLoader.h"
#include "Snake.h"

//#include <direct.h>
#include "Timing.h"

//Functions
int main();
void splashScreen();
void Initialise();
void render();
void updateView();
void setupShaders();
void setupView();
void setupEntities();
void updateViewProps();
void updateActiveEntities();
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
void updateSnakes();

//Sphere reading constants
unsigned int sphereVertexCount = 119406;
unsigned int sphereIndexCount = 238800;
unsigned int sphereDistanceCount = 39802;

//Time etc. management variables
float DisplayFrameTime;
float yOffset = 1.0f;
int DisplayFrame;
float updateInterval = 0.001f;
float *timey = new float;
float timeSinceWave[3] = { 0, 0, 0 };
float timeDiff[3] = { 0, 0, 0 };
double startingTime, changeFrameTime;
float timeytime = 0;
Timer *changeFrameTimer;

//State of program
int phase = 0;
int clickedExit = 0;
int wireframe = 0, fillType = GL_LINE;
bool sendWave = false;
float waveAlpha = 0.0;
static bool clipOn = false;
bool clippy = false;
bool pauseRender = false;

//Initialisation state
bool InitialisationComplete = false;
std::string currentJob = "";

//Input variables
MouseHandler *mouse;
static glm::vec2 previousMousePos;
static bool isMouseHeld;
static float lastMouseClock;
KeyHandler keyHandler;
bool keyDown[GLFW_KEY_Z - GLFW_KEY_A];
keyType *forwardKey, *backKey, *leftKey, *rightKey, *spaceKey, *upCursor, *downCursor;
Terrain* terrain;

//Audio variables
AudioPlate* audioPlate; 
WAV_File* testFile;
AudioEntity* music;


BoilerPlate::BoilerPlate boiler;

BoilerPlate::Properties::GLFWproperties glfwProps;
BoilerPlate::Properties::GLEWproperties glewProps;

//Camera settings
BoilerPlate::Maths::vec3 eye(0, 0, 0);
glm::mat4 projection;
glm::mat4 cam;
Camera camera;
glm::vec4 clipPlane = glm::vec4(0, -1, 0, 8);
float FOV = 90;
float Near_Plane = 0.1f;
float Far_Plane = 1000.0f;
float aspectRatio;

PostProcessing *postProcessPipeline;
ContrastAdjustModule::ContrastData *cData;
BrightnessAdjustModule::BrightnessData *bData;
SaturationAdjustModule::SaturationData *sData;
BloomEffectModule::BloomEffectData *bloomData;

Framebuffer *mainFrameBuffer, *toScreenBuffer, *reflectionFBO, *refractionFBO;
BoilerPlate::Shaders::Shader sphereShader, groundShader2, normalShader, skyboxShader, entityShader, guiShader, waterShader, snakeShader;

BoilerPlate::Physics::DynamicEntity *player, physicSnakes[30];

Entity *sphere, *ground, *scissors, *testTex, *snake;
uint particle_timing_id, physics_timing_id, snake_timing_id, performance_checking_id;
BatchUnit *groundGroup, *scissorUnit, *treeUnit, *snakeUnit;

std::vector<Snake*> snakes;
Skybox *skybox;
Texture *groundTex;
Water *water;
WaterPackage *waterPackage;

BufferObject *sphereVerticesBO;
uniformData *cameraUBO, *waveformUBO, *lightingUBO, *clipUBO;
uniformData timeUniform, projectionUniform, viewUniform, timeDiffUniform, waveformUniform, boxMVP;
RenderMode sphereRenderer, normalRenderer, skyboxRenderer, groundRenderer, scissorRenderer, testTexRenderer, waterRenderer, snakeRenderer;
Renderer renderer, guiRenderer;


//Structs etc
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

BoilerPlate::Properties::displayProperties displayProps = {
	1920,		//width
	1080,		//height
	"Test",		//Title
	NULL,		//Monitor
	NULL,		//Shared context
	false		//Fullscreen
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

