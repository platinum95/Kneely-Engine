// OpenGL_Testing.cpp : Defines the entry point for the console application.
//


#include "OpenGL_Testing.h"

#include <sys/stat.h>
#define NO_AUDIO
unsigned int ScissorvertexCount = 93132;
unsigned int ScissorindexCount = 136572;

#define GLEW_STATIC

//65 or 66

void ShowFps(GLFWwindow *pWindow) {
	double WorldTime = glfwGetTime();
	double delta = WorldTime - DisplayFrameTime;
	DisplayFrame++;
	if (delta >= 1.0) {
		double fps = double(DisplayFrame) / delta;

		std::stringstream ss;
		ss << "FPS: " << fps;
	//	ss  << player->position.x << ", " << player->position.z;
//		glfwSetWindowTitle(pWindow, ss.str().c_str());

		DisplayFrame = 0;
		DisplayFrameTime = WorldTime;
	}
}


int main(){
	int beatCount = 63;
	changeFrameTimer = new Timer(EXT_TIMER, false);
	changeFrameTimer->addEvent(new TimerEvent(&changeWireframe, nullptr, 11.52110));
	//changeFrameTimer->addEvent(new TimerEvent(&enableClip, nullptr, 75.2));
	//changeFrameTimer->addEvent(new TimerEvent(&disableSphere, nullptr, 76));
	init();
	startingTime = glfwGetTime();
	changeFrameTime = startingTime + 5;
	glfwSetTime(10.0);
	changeFrameTimer->setTime(glfwGetTime());
	changeFrameTimer->start();
#ifndef NO_AUDIO
	music->playSound(*testFile->buffer);
#endif
	*timey = 0;
	double lastTime = clock();


	while (!glfwWindowShouldClose(glfwProps.window) && clickedExit < 1) {
		double timeSince = (clock() - lastTime) / CLOCKS_PER_SEC;
		lastTime = clock();
		changeFrameTimer->setTime(glfwGetTime());
		double fps = 1.0 / timeSince;
		ShowFps(glfwProps.window);

		if (sendWave) {
			*timey = glfwGetTime() * 5.3;
			double diff = *timey - (timeSinceWave[0] + 5.3);
			//std::cout << glfwGetTime() << std::endl;
			if (clippy) {
				float startingPoint = timeDiff[0] * 40.0f;
				float endingPoint = startingPoint - 10.0f;
				float radi = endingPoint / 50.0f;
				float disti = 50.0f - (cos(radi) * 50.0f);

				clipPlane.w = disti - 50.0f;
			}
			if (diff > 0) {
				beatCount++;
				if (beatCount == 65)
					enableClip(NULL);
				if (beatCount == 66)
					disableSphere(NULL);
				timeSinceWave[0] = *timey;
				timeSinceWave[1] = timeSinceWave[0] + 3;
				if (waveAlpha < 1)
					waveAlpha += 0.0625;
			}
		}
		timeDiff[0] = *timey - timeSinceWave[0];
		updateActiveEntities();
		render();
		glfwPollEvents();
		keyHandler.update(glfwProps.window);
		updateView();
	}
	cleanup();
	changeFrameTimer->end();
    return 0;
}

void updateActiveEntities() {
	for (BatchUnit *BU : terrain->chunkEntity->units) {
			glm::vec4 origin = glm::vec4(0, 0, 0, 1);
			origin = BU->transformationMatrix * origin;
			float clip = glm::dot(origin, camera.plane);
			if (clip < -1) {
			//	BU->active = false;
			}
			//else
			//	BU->active = true;
		}

}

void updateView() {
	updateViewProps();
	checkMouse(0);
	if(terrain->chunkEntity->active != groundOn){
	//	terrain->chunkEntity->active = groundOn;
	}
	
	
	lighting.position[0] = camera.position.x;
	lighting.position[1] = camera.position.y;
	lighting.position[2] = camera.position.z;
	lighting.colour[0] = 1.0f;//(sin(glfwGetTime()) + 1.0f)/2.0f * 0.8f + 0.2f;
	lighting.colour[1] = 1.0f;//(cos(glfwGetTime()) + 1.0f)/2.0f + 0.2f;
	lighting.colour[2] = 1.0f;// (sin(glfwGetTime()* 16.0f) + 1.0f) / 2.2f + 0.2f;
	
	glm::vec4 direction = camera.direction;
	float amount = 500.0f;
	if (forwardKey->isDownDown) {
		player->addForce(glm::vec3(direction.x * amount, direction.y *amount, direction.z * amount));
	}
	if (backKey->isDownDown) {
		player->addForce(glm::vec3(direction.x * -amount, direction.y *-amount, direction.z * -amount));
		//camera.moveZ(-0.5);
	}
	glm::vec3 rDir = glm::vec3(direction);
	rDir = glm::rotateY(rDir, glm::pi<float>() * 0.5f);
	if (rightKey->isDownDown) {
		player->addForce(glm::vec3(rDir.x * -amount, rDir.y *-amount, rDir.z * -amount));
	}
	if (leftKey->isDownDown) {
		player->addForce(glm::vec3(rDir.x * amount, rDir.y *amount, rDir.z * amount));
	}
	if (spaceKey->isDownDown) {
		player->addForce(glm::vec3(0, amount, 0));
	}
	if (upCursor->isDownDown) {
		PhysicsCock->addForce(glm::vec3(1000, 0, 0));
	}
	if (downCursor->isDownDown) {
		PhysicsCock->addForce(glm::vec3(-1000, 0, 0));
	}
	
	std::chrono::nanoseconds physics_diff_nano = Timing::getTimeDiffNanos(physics_timing_id);
	double physics_diff_double = physics_diff_nano.count() / 1000000000.0;

	terrain->updateState(camera.position);
	player->update(physics_diff_double);
	PhysicsCock->update(physics_diff_double);
	camera.position = player->position;
	int height;
	if (player->position.x > 127.9f && player->position.x < 128.0f && player->position.z > 1113)
		height = terrain->getHeight(camera.position);

	if(player->position.y < terrain->getHeight(camera.position) + yOffset)
		player->position.y += (terrain->getHeight(player->position) + yOffset) - player->position.y;


	//roll is pitch - x
	//yaw is yaw -y
	

	camera.updateCam();
	if (camera.position.x > 127.9f && camera.position.x < 128.0f && camera.position.z > 1113)
		height = terrain->getHeight(camera.position);

	if (camera.position.y < terrain->getHeight(camera.position) + yOffset)
		camera.position.y += (terrain->getHeight(camera.position) + yOffset) - camera.position.y;

	std::chrono::nanoseconds ps_diff_nano = Timing::getTimeDiffNanos(particle_timing_id);
	double ps_diff_double = ps_diff_nano.count() / 1000000000.0;

	testPSystem->UpdateParticleSystem(ps_diff_double);
}

void render() {	
	if (!pauseRender) {
		glEnable(GL_CLIP_DISTANCE0);
		water->waveOffset += 0.01;
		updateView();

		renderer.clear();

		mainFrameBuffer->bindBuffer();
		mainFrameBuffer->clear();
		renderer.Render();

		mainFrameBuffer->unbindBuffer();
		postProcessPipeline->getOutputBuffer()->clearBlack();
		postProcessPipeline->Render();

		guiRenderer.Render();

		glfwSwapBuffers(glfwProps.window);	//Swap the window buffers
	}
}

void init() {
	_mkdir("temp");
	glfwProps = boiler.startGLFW(displayProps);
	glewExperimental = GL_TRUE;
	glewProps = boiler.startGLEW();

	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_COLOR_MATERIAL);
	

	audioPlate = new AudioPlate();
	testFile = new WAV_File();
	*testFile = LoadAudio::loadWav("./res/sounds/stm.wav");

	audioPlate->loadAudio(testFile);
	audioPlate->setListenerData();
	music = new AudioEntity();

	LoadAudio::freeData(*testFile);

	BoilerPlate::Physics::PhysicsConstants::gravity = -9.81f;
	BoilerPlate::Physics::PhysicsConstants::drag_coef = 0.001f;


	setupView();
	setupShaders();



	
	wireframe = GL_FILL;
	for (int i = 0; i < 1544; i++) {
		heartbeat[i] *= 2;
		heartbeat[i] -= 1;
	}

	setupEntities();

	*timey = 0;
	forwardKey = new keyType(keyType(GLFW_KEY_W));
	backKey = new keyType(GLFW_KEY_S);
	leftKey = new keyType(GLFW_KEY_A);
	rightKey = new keyType(GLFW_KEY_D);
	spaceKey = new keyType(GLFW_KEY_SPACE);
	upCursor = new keyType(GLFW_KEY_UP);
	downCursor = new keyType(GLFW_KEY_DOWN);
	keyHandler = KeyHandler();
	keyHandler.addToList(keyType(&(camera.yaw),			0.01, GLFW_KEY_Q, GLFW_KEY_E, GLFW_PRESS));
	keyHandler.addToList(keyType(&(yOffset),	0.1, GLFW_KEY_R, GLFW_KEY_F, GLFW_PRESS));
	keyHandler.addToList(keyType(&(camera.roll),		0.01, GLFW_KEY_Z, GLFW_KEY_X, GLFW_PRESS));
	keyHandler.addToList(keyType(&(FOV),				2, GLFW_KEY_1, GLFW_KEY_2, GLFW_CLICK));
	keyHandler.addToList(keyType(&(clickedExit),		1, GLFW_KEY_ESCAPE, GLFW_KEY_F1, GLFW_CLICK));
	keyHandler.addToList(keyType(&(lighting.position[1]), 1, GLFW_KEY_Y, GLFW_KEY_U, GLFW_CLICK));
	keyHandler.addToList(keyType(nullptr,				&changeWireframe2, GLFW_KEY_H, GLFW_CLICK));
	//keyHandler.addToList(keyType(nullptr,				&disableGround, GLFW_KEY_L, GLFW_CLICK));
	keyHandler.addToList(keyType(&(sData->amount), 0.1, GLFW_KEY_P, GLFW_KEY_SEMICOLON , GLFW_CLICK));
	keyHandler.addToList(keyType(&(cData->amount), 0.1, GLFW_KEY_O, GLFW_KEY_L, GLFW_CLICK));
	keyHandler.addToList(keyType(&(bData->amount), 0.1, GLFW_KEY_I, GLFW_KEY_K, GLFW_CLICK));
	keyHandler.addToList(forwardKey);
	keyHandler.addToList(backKey);
	keyHandler.addToList(leftKey);
	keyHandler.addToList(rightKey);
	keyHandler.addToList(spaceKey);
	keyHandler.addToList(upCursor);
	keyHandler.addToList(downCursor);

	mouse = new MouseHandler();
	//mouse->addCallback(&checkMouse, GLFW_MOUSE_BUTTON_LEFT);

	Timing::genTimingID(&particle_timing_id, 1);
	Timing::genTimingID(&physics_timing_id, 1);
	Timing::start(particle_timing_id);
	Timing::start(physics_timing_id);
}

void setupShaders() {

	cameraUBO = new uniformData(GL_UNIFORM_BUFFER, &camera.camera_data, (2 * 16) * sizeof(float) , "camera_data");
	cameraUBO->active = true;
	cameraUBO->binding_index = 0;
	int j = -1;
	for (int i = 0; i < 1544; i++) {
		heart_wave.offsetwave[i] = heartbeat[i] * 2;
		heart_wave.offsetwave[i] -= 1;
		int colourNum = heartbeat[i] * 16777215;
		int red = (colourNum >> 16) & 0xFF;
		int green = (colourNum >> 8) & 0xFF;
		int blue = colourNum & 0xFF;
		heart_wave.colourwave[++j] = red / 255.0f;
		heart_wave.colourwave[++j] = green / 255.0f;
		heart_wave.colourwave[++j] = blue / 255.0f;
		heart_wave.colourwave[++j] = 1;
		heart_wave.colourwave[++j] = 1;
		heart_wave.colourwave[++j] = 1;
		heart_wave.colourwave[++j] = 1;
	}

	waveformUBO = new uniformData(GL_UNIFORM_BUFFER, &heart_wave, 1544 * 8 * sizeof(float) , "waveform");
	waveformUBO->active = true;
	waveformUBO->binding_index = 1;

	lightingUBO = new uniformData(GL_UNIFORM_BUFFER, &lighting, 8 * sizeof(float), "light");
	lightingUBO->active = true;
	lightingUBO->binding_index = 2;

	clipUBO = new uniformData(GL_UNIFORM_BUFFER, &clip_plane, 4 * sizeof(float), "clip_plane");
	clipUBO->active = true;
	clipUBO->binding_index = 3;
	
	//Dump(&heart_wave, 1544 * 5 * sizeof(float));
	BoilerPlate::Shaders::Shader::addUBO(cameraUBO);
	BoilerPlate::Shaders::Shader::addUBO(waveformUBO);
	BoilerPlate::Shaders::Shader::addUBO(lightingUBO);
	BoilerPlate::Shaders::Shader::addUBO(clipUBO);
	BoilerPlate::Shaders::Shader::generateUBO();

	
	sphereShader.RegisterAttribute("position", 0);
	sphereShader.RegisterAttribute("normal", 1);
	sphereShader.RegisterAttribute("distance", 2);
	sphereShader.RegisterUniform("timeDiff");
	sphereShader.RegisterUniform("waveForm");
	sphereShader.RegisterUniform("alpha");
	sphereShader.RegisterUniform("clippy");
	sphereShader.registerUBO(cameraUBO);
	sphereShader.registerUBO(waveformUBO);
	sphereShader.registerUBO(clipUBO);
	sphereShader.addShaderType(planeVertexPath, GL_VERTEX_SHADER);
	sphereShader.addShaderType(planeFragmentPath, GL_FRAGMENT_SHADER);
	sphereShader.LoadShader();

	skyboxShader.RegisterAttribute("position", 0);
	skyboxShader.RegisterUniform("cubeMap");
	skyboxShader.registerUBO(cameraUBO);
	skyboxShader.registerUBO(clipUBO);
	skyboxShader.addShaderType(skyboxVPath, GL_VERTEX_SHADER);
	skyboxShader.addShaderType(skyboxFPath, GL_FRAGMENT_SHADER);
	skyboxShader.LoadShader();

	entityShader.RegisterAttribute("position", 0);
	entityShader.RegisterAttribute("normal", 1);
	entityShader.RegisterAttribute("texCoords", 2);
	entityShader.RegisterUniform("translation");
	entityShader.RegisterUniform("test");
	entityShader.registerUBO(cameraUBO);
	entityShader.registerUBO(lightingUBO);
	entityShader.registerUBO(clipUBO);
	entityShader.addShaderType(entityVPath, GL_VERTEX_SHADER);
	entityShader.addShaderType(entityFPath, GL_FRAGMENT_SHADER);
	entityShader.LoadShader();

	guiShader.RegisterAttribute("position", 0);
	guiShader.RegisterAttribute("texCoords", 1);
	guiShader.RegisterUniform("tex");
	guiShader.RegisterTexture("tex", 0);
	guiShader.addShaderType(guiVPath, GL_VERTEX_SHADER);
	guiShader.addShaderType(guiFPath, GL_FRAGMENT_SHADER);
	guiShader.LoadShader();

	guiShader.useShader();
	int zero = 0, one = 1, two = 2, three = 3;
	BoilerPlate::Shaders::Shader::loadInt2(guiShader.uniformTable[0]->uniformLocation, &zero);
	guiShader.stopShader();

	


	groundShader2.RegisterAttribute("position", 0);
	groundShader2.RegisterAttribute("height", 1);
	groundShader2.RegisterAttribute("textureCoords", 2);
	groundShader2.RegisterAttribute("normal", 3);
	groundShader2.RegisterUniform("translation");
	groundShader2.RegisterUniform("tex");
	groundShader2.RegisterUniform("clippy");
	groundShader2.RegisterUniform("clipPlane");
	groundShader2.registerUBO(cameraUBO);
	groundShader2.registerUBO(lightingUBO);
	groundShader2.registerUBO(clipUBO);
	groundShader2.addShaderType("./res/shaders/groundVShader2.glsl", GL_VERTEX_SHADER);
	groundShader2.addShaderType("./res/shaders/groundFShader2.glsl", GL_FRAGMENT_SHADER);
	groundShader2.LoadShader();


	BoilerPlate::Shaders::Shader::loadUBOData(*waveformUBO);
	waveformUBO->active = false;

	BoilerPlate::Shaders::Shader::loadUBOData(*lightingUBO);
	BoilerPlate::Shaders::Shader::loadUBOData(*clipUBO);

	
	normalShader.RegisterAttribute("position", 0);
	normalShader.RegisterAttribute("normal", 3);
	normalShader.addShaderType(normalVertexPath, GL_VERTEX_SHADER);
	normalShader.addShaderType(normalFragmentPath, GL_FRAGMENT_SHADER);
	normalShader.addShaderType(normalGeometryPath, GL_GEOMETRY_SHADER);
	normalShader.LoadShader();

	

	mainFrameBuffer = new Framebuffer();
	mainFrameBuffer->createBuffer(2);
	mainFrameBuffer->genDepthBufferAttachment();
	mainFrameBuffer->genDepthTextureAttachment();
	mainFrameBuffer->genTextureAttachment();
	mainFrameBuffer->genTextureAttachment();
	mainFrameBuffer->unbindBuffer();

	toScreenBuffer = new Framebuffer();
	toScreenBuffer->createBuffer(2);
	toScreenBuffer->genDepthBufferAttachment();
	toScreenBuffer->genDepthTextureAttachment();
	toScreenBuffer->genTextureAttachment();
	toScreenBuffer->unbindBuffer();

	postProcessPipeline = new PostProcessing();
	cData = new ContrastAdjustModule::ContrastData(1);
	bData = new BrightnessAdjustModule::BrightnessData(0);
	sData = new SaturationAdjustModule::SaturationData(1.3);
	bloomData = new BloomEffectModule::BloomEffectData;
	postProcessPipeline->RegisterEffect(ContrastAdjust, cData);
	postProcessPipeline->RegisterEffect(BrightnessAdjust, bData);
	postProcessPipeline->RegisterEffect(SaturationAdjust, sData);
	postProcessPipeline->RegisterEffect(BloomEffect, bloomData);
	Framebuffer *testBuffer = new Framebuffer();
	testBuffer->genTextureAttachment();
	postProcessPipeline->generatePostProcess(mainFrameBuffer);


}


void setupView() {
	aspectRatio = (float)displayProps.width / (float)displayProps.height;


	float frustum_length = Far_Plane - Near_Plane;

	float fov2 = glm::radians(FOV);
	projection = glm::mat4();
	projection = glm::perspective(fov2, aspectRatio, 0.1f, 1000.0f);

	camera.projection = projection;
	camera.position.z = 20;
	camera.position.x = 0;
	camera.position.y = 0;

	camera.pitch = 0;
	camera.roll = 3.14 / 2;
	camera.yaw = 3.14;

	player = new BoilerPlate::Physics::DynamicEntity();
	PhysicsCock = new BoilerPlate::Physics::DynamicEntity();

	lighting.position[0] = 0.0f;
	lighting.position[1] = 90.0f;
	lighting.position[2] = 0.0f;
	lighting.position[3] = 1.0f;

	lighting.colour[0] = 1.0f;
	lighting.colour[1] = 1.0f;
	lighting.colour[2] = 1.0f;
	lighting.colour[3] = 1.0f;

}

void updateViewProps() {
	float fov2 = glm::radians(FOV);
	projection = glm::perspective(fov2, aspectRatio, Near_Plane, Far_Plane);
}


void checkMouse(int action) {
	if (mouse->isDown(GLFW_MOUSE_BUTTON_LEFT)) {
		if (!isHeld) {
			isHeld = true;
			previousPos = mouse->getMousePosition();
			lastClock = clock();
			return;
		}
		float currentClock = clock();
		float clockDiff = currentClock - lastClock;

		glm::vec2 currentPos = mouse->getMousePosition();
		glm::vec2 posVector = currentPos - previousPos;
		clockDiff = clockDiff == 0 ? 1 : clockDiff;
		float magsPerClock = posVector.length() / clockDiff;
		previousPos = currentPos;
		lastClock = currentClock;
		camera.moveBy(posVector * (magsPerClock * 0.01f));
	}
	if (!mouse->isDown(0) && isHeld) {
		previousPos = glm::vec2(0, 0);
		isHeld = false;
	}

}

BufferObject genTexShit() {
	float* texCoordData = new float[256 * 256 * 2];
	int arpos = 0;
	for (int i = 0; i < 256; i++) { //x
		float ival = (float)i / 15.0f;
		for (int j = 0; j < 256; j++) { //y
			texCoordData[arpos] = ival;
			arpos++;
			texCoordData[arpos] = (float)j / 15.0f;
			arpos++;
		}
	}
	BufferObject bo = BufferObject();
	bo.data = texCoordData;
	bo.dimension = 2;
	bo.size = 256 * 256 * 2;
	bo.unitSize = 4;
	return bo;
}



/*
float* calcNorm(int x, int z, ImageData data) {
	unsigned char* idata = (unsigned char*)data.data;
	float heightL = getHeight(x - 1, z, idata);
	float heightR = getHeight(x + 1, z, idata);
	float heightD = getHeight(x, z - 1, idata);
	float heightU = getHeight(x, z + 1, idata);
	glm::vec3 norm = glm::vec3(heightL - heightR, 2.0f, heightD - heightU);
	norm = glm::normalize(norm);
	float *out = new float[3];
	out[0] = norm.x;
	out[1] = norm.y;
	out[2] = norm.z;
	return &out[0];
}

BufferObject *getNormals(BoilerPlate::Properties::EntityProperties obj, ImageData data) {
	BufferObject* norms = new BufferObject(floatProps, 2, obj.vertexSize, obj.vertices);
	unsigned char* idata = (unsigned char*)data.data;

	norms->data = new float[norms->size];
	float* datah = (float*) norms->data;
	uint size = obj.indexSize;
	int arpos = -1;
	for (int i = 0; i < 256; i++) { //x
		for (int j = 0; j < 256; j++) { //y
			float *normc = calcNorm(i, j, data);//height;
			datah[++arpos] = normc[0];
			datah[++arpos] = normc[1];
			datah[++arpos] = normc[2];
		}
	}


	return norms;

}

*/
void setupEntities() {


	BinaryLoader loader;
	std::vector<BufferObject*> testList = loader.readFile("./res/entities/sphereEntity.bin");
//	loader.createFile("./tree2.obj", "./res/entities/tree2.bin");


	testList[1]->update(floatProps, 0);
	testList[2]->update(floatProps, 1);
	testList[3]->update(floatProps, 2);

	
	renderer = Renderer();
	
	skybox = new Skybox(skyboxShader);
	skybox->genTextures(skyboxFiles2);
	renderer.addToRenderer(&skybox->renderMode);


	sphere = new Entity();
	sphere->registerBufferObject(testList[1]);
	sphere->registerBufferObject(testList[3]);
	sphere->registerBufferObject(testList[2]);
	sphere->createEntity(testList[0]->data, testList[0]->size);
	sphereRenderer = RenderMode(GL_TRIANGLES, sphereShader);
	sphereRenderer.entityList.push_back(sphere);
	sphere->uniforms.push_back(uniformData(sphereShader.uniformTable.at(0)->uniformLocation,
			timeDiff, &BoilerPlate::Shaders::Shader::loadFloatArray));
	sphere->uniforms.push_back(uniformData(sphereShader.uniformTable.at(2)->uniformLocation,
			&waveAlpha, &BoilerPlate::Shaders::Shader::loadFloat2));
	sphere->uniforms.push_back(uniformData(sphereShader.uniformTable.at(3)->uniformLocation,
			&clippy, &BoilerPlate::Shaders::Shader::loadBool2));
	sphere->units.push_back(new BatchUnit());
	renderer.addToRenderer(&sphereRenderer);

	loader.freeData(testList);

	

//  ImageLoader::PNGtoRAW("./res/images/waterDUDV.png", "./res/images/waterDUDV.raw");
//	ImageLoader::PNGtoRAW("./res/images/water_normal.png", "./res/images/water_normal.raw");

	ImageData grass = ImageLoader::loadRAW("./res/images/grass.raw");
	//ImageData grass = ImageLoader::loadPNG("./res/images/imogen.png");
	groundTex = new Texture(grass, GL_TEXTURE_2D);
	groundTex->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	groundTex->registerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	groundTex->registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	groundTex->registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	groundTex->textureBank = GL_TEXTURE0;
	groundTex->updateTexture();
	ImageLoader::freeData(grass);


	
	std::vector<BufferObject*> groundTest = loader.readFile("./res/entities/groundA.bin");
	groundTest[1]->update(floatProps, 0);
	groundTest[2]->update(floatProps, 2);
	groundTest[3]->update(floatProps, 1);



	TerrainGenerator::generateSeed();

	std::vector<BufferObject*> groundTest2 = loader.readFile("./res/entities/groundA.bin");
	groundTest2[3]->update(floatProps, 2);

	

	terrain = new Terrain(256, 256, "./res/entities/groundA.bin", groundShader2);
	terrain->chunkEntity->registerBufferObject(groundTest2[3]);
	terrain->chunkEntity->textures.push_back(groundTex);
	terrain->chunkEntity->uniforms.push_back(uniformData(groundShader2.uniformTable.at(2)->uniformLocation,
			&clippy, &BoilerPlate::Shaders::Shader::loadBool2));
	terrain->chunkEntity->uniforms.push_back(uniformData(groundShader2.uniformTable.at(3)->uniformLocation,
			&clipPlane, &BoilerPlate::Shaders::Shader::loadVec42));
	terrain->chunkEntity->createEntity(groundTest2[0]->data, groundTest2[0]->size);
	terrain->chunkEntity->active = false;
	terrain->chunkEntity->transformationLocation = groundShader2.uniformTable.at(0)->uniformLocation;




	double start = clock();
	std::cout << "Start gen: " << start << "\n";
//	terrain->generateChunk(0, 0, 1, 2, 7);
//	terrain->generateChunk(1, 0, 1, 2, 7, 0.1f, 1, 2);
//	terrain->generateChunk(2, 0, 0.1f, 1, 2);

	for(int i = -4; i < 5; i++){
		for(int j = -4; j < 5; j++){
			terrain->registerChunk(i, j);
		}
	}

	//terrain->generateChunk(0, 0);

	terrain->updateActiveChunks();
	terrain->updateCurrentUnits();

	renderer.addToRenderer(terrain->TerrainRenderMode);

	normalRenderer = RenderMode(GL_TRIANGLES, normalShader);
	normalRenderer.entityList.push_back(terrain->chunkEntity);
	normalRenderer.p = terrain->TerrainRenderMode->p;
//	renderer.addToRenderer(&normalRenderer);
	WaterProperties wProps;
	wProps.cameraUBO = cameraUBO;
	wProps.lightingUBO = lightingUBO;
	water = new Water(wProps);
	water->waterPackage->renderer = &renderer;
	water->waterPackage->camera = &camera;
	water->waterPackage->clip_plane = clip_plane.plane;
	renderer.addToRenderer(water->waterRenderMode);

	scissors = new Entity();
	scissorUnit = new BatchUnit();
	BufferObject* scissorVerts = new BufferObject(floatProps, 0, ScissorvertexCount, ScissorVertices);
	BufferObject* scissorNorms = new BufferObject(floatProps, 1, ScissorvertexCount, scissorNormals);
	scissors->registerBufferObject(scissorVerts);
	scissors->registerBufferObject(scissorNorms);
	scissors->createEntity(scissorIndices, ScissorindexCount);
	scissors->uniforms.push_back(uniformData(entityShader.uniformTable.at(0)->uniformLocation, &scissorUnit->transformationMatrix,
			BoilerPlate::Shaders::Shader::loadMat42));
	scissorRenderer = RenderMode(GL_TRIANGLES, entityShader);
	scissorRenderer.entityList.push_back(scissors);
	scissors->units.push_back(scissorUnit);
	renderer.addToRenderer(&scissorRenderer);



	Texture *mainTexture = new Texture();
	mainTexture->texID = postProcessPipeline->getOutputBuffer()->colour_attachments[0];//mainFrameBuffer->colour_attachments[0];
	mainTexture->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	mainTexture->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	mainTexture->target = GL_TEXTURE_2D;
	mainTexture->textureBank = GL_TEXTURE0;

	testTex = new Entity();
	BufferObject* testVerts = new BufferObject(floatPropsTwoD, 0, 8, testTexVert);
	BufferObject* testTexs = new BufferObject(floatPropsTwoD, 1, 8, testTexTex);
	testTex->registerBufferObject(testVerts);
	testTex->registerBufferObject(testTexs);
	testTex->textures.push_back(mainTexture);
	testTex->createEntity(testTexIndices, 6);
	testTex->units.push_back(new BatchUnit());
	testTexRenderer = RenderMode(GL_TRIANGLES, guiShader);
	testTexRenderer.entityList.push_back(testTex);
	guiRenderer.addToRenderer(&testTexRenderer);

	testPSystem = new ParticleSystem(30000, cameraUBO);
	renderer.addToRenderer(testPSystem->getRenderMode());
}

void cleanup() {
	delete cameraUBO;
	delete waveformUBO;
	sphere->cleanUp();
	delete sphere;
	remove("./temp");
	delete water;
}

void changeWireframe(void* nothiung) {
	sendWave = true;
}

void changeWireframe2(void* nothiung) {
	fillType = fillType == GL_LINE ? GL_FILL : GL_LINE;
	glPolygonMode(GL_FRONT_AND_BACK, fillType);
}

void enableClip(void* nothing) {
	clippy = true;
	clipOn = true;
	
	terrain->chunkEntity->active = true;
	
}

void disableSphere(void*) {
	sphere->active = false;
	clippy = false;
}
static void forwardClick(void*) {
	forwardDown = true;
}
static void forwardRelease(void*) {
	forwardDown = false;
}

void disableGround(void*){
	//groundOn = !groundOn;
	pauseRender = !pauseRender;
}
