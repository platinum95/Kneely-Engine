// OpenGL_Testing.cpp : Defines the entry point for the console application.
//


#include "OpenGL_Testing.h"

#include <sys/stat.h>
#define NO_AUDIO
unsigned int ScissorvertexCount = 93132;
unsigned int ScissorindexCount = 136572;

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
		DisplayFrameTime = (float) WorldTime;
	}
}

int main(){
	glfwProps = boiler.startGLFW(displayProps);
	glewExperimental = GL_TRUE;
	glewProps = boiler.startGLEW();

	
//	std::thread InitialisationThread(Initialise);
//	InitialisationThread.detach();
	splashScreen();
	Initialise();
	while (!InitialisationComplete) {
		;
	}

#ifndef NO_AUDIO
	music->playSound(*testFile->buffer);
#endif
	double lastTime = clock();
	int beatCount = 63;
	while (!glfwWindowShouldClose(glfwProps.window) && clickedExit < 1) {
		double timeSince = (clock() - lastTime) / CLOCKS_PER_SEC;
		lastTime = clock();
		changeFrameTimer->setTime(glfwGetTime());
		double fps = 1.0 / timeSince;
		ShowFps(glfwProps.window);

		if (sendWave) {
			*timey = (float) glfwGetTime() * 5.3f;
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
				if (beatCount == 66) {
					disableSphere(NULL);
					changeFrameTimer->end();
				}
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

void render() {	
	if (!pauseRender) {
		glEnable(GL_CLIP_DISTANCE0);
		water->waveOffset += 0.01f;
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

void Initialise() {
	Timing::genTimingID(&performance_checking_id, 1);
	Timing::start(performance_checking_id);
	double performance_time = 0;
	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "Starting at: " << performance_time << std::endl;

	changeFrameTimer = new Timer(EXT_TIMER, true);
	changeFrameTimer->addEvent(new TimerEvent(&changeWireframe, nullptr, 11.52110));
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_COLOR_MATERIAL);
	_mkdir("temp");
	
	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "First bits at: " << performance_time << std::endl;

	audioPlate = new AudioPlate();
	testFile = new WAV_File();
	*testFile = LoadAudio::loadWav("./res/sounds/stm.wav");

	audioPlate->loadAudio(testFile);
	audioPlate->setListenerData();
	music = new AudioEntity();
	

	LoadAudio::freeData(*testFile);
	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "Audio: " << performance_time << std::endl;

	BoilerPlate::Physics::PhysicsConstants::gravity = -9.81f;
	BoilerPlate::Physics::PhysicsConstants::drag_coef = 0.001f;


	setupView();
	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "View: " << performance_time << std::endl;

	setupShaders();

	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "Shaders: " << performance_time << std::endl;

	
	wireframe = GL_FILL;
	for (int i = 0; i < 1544; i++) {
		heartbeat[i] *= 2;
		heartbeat[i] -= 1;
	}
	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "Heartbeat: " << performance_time << std::endl;

	setupEntities();
	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "Entities: " << performance_time << std::endl;

	*timey = 0;
	forwardKey = new keyType(keyType(GLFW_KEY_W));
	backKey = new keyType(GLFW_KEY_S);
	leftKey = new keyType(GLFW_KEY_A);
	rightKey = new keyType(GLFW_KEY_D);
	spaceKey = new keyType(GLFW_KEY_SPACE);
	upCursor = new keyType(GLFW_KEY_UP);
	downCursor = new keyType(GLFW_KEY_DOWN);
	keyHandler = KeyHandler();
	keyHandler.addToList(keyType(&(camera.yaw),			0.01f, GLFW_KEY_Q, GLFW_KEY_E, GLFW_PRESS));
	keyHandler.addToList(keyType(&(yOffset),	0.1f, GLFW_KEY_R, GLFW_KEY_F, GLFW_PRESS));
	keyHandler.addToList(keyType(&(camera.roll),		0.01f, GLFW_KEY_Z, GLFW_KEY_X, GLFW_PRESS));
	keyHandler.addToList(keyType(&(FOV),				2.0f, GLFW_KEY_1, GLFW_KEY_2, GLFW_CLICK));
	keyHandler.addToList(keyType(&(clickedExit),		1.0f, GLFW_KEY_ESCAPE, GLFW_KEY_F1, GLFW_CLICK));
	keyHandler.addToList(keyType(&(lighting.position[1]), 1.0f, GLFW_KEY_Y, GLFW_KEY_U, GLFW_CLICK));
	keyHandler.addToList(keyType(nullptr,				&changeWireframe2, GLFW_KEY_H, GLFW_CLICK));
	//keyHandler.addToList(keyType(nullptr,				&disableGround, GLFW_KEY_L, GLFW_CLICK));
	keyHandler.addToList(keyType(&(sData->amount), 0.1f, GLFW_KEY_P, GLFW_KEY_SEMICOLON , GLFW_CLICK));
	keyHandler.addToList(keyType(&(cData->amount), 0.1f, GLFW_KEY_O, GLFW_KEY_L, GLFW_CLICK));
	keyHandler.addToList(keyType(&(bData->amount), 0.1f, GLFW_KEY_I, GLFW_KEY_K, GLFW_CLICK));
	keyHandler.addToList(forwardKey);
	keyHandler.addToList(backKey);
	keyHandler.addToList(leftKey);
	keyHandler.addToList(rightKey);
	keyHandler.addToList(spaceKey);
	keyHandler.addToList(upCursor);
	keyHandler.addToList(downCursor);

	mouse = new MouseHandler();
	//mouse->addCallback(&checkMouse, GLFW_MOUSE_BUTTON_LEFT);
	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "Inputs: " << performance_time << std::endl;

	Timing::genTimingID(&particle_timing_id, 1);
	Timing::genTimingID(&physics_timing_id, 1);
	Timing::genTimingID(&snake_timing_id, 1);
	Timing::start(particle_timing_id);
	Timing::start(physics_timing_id);
	Timing::start(snake_timing_id);
	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "Timers: " << performance_time << std::endl;

	startingTime = glfwGetTime();
	changeFrameTime = startingTime + 5;
	glfwSetTime(10.0);
	changeFrameTimer->setTime(glfwGetTime());
	changeFrameTimer->start();

	*timey = 0;
	InitialisationComplete = true;
	performance_time = Timing::getTimeDiff<std::chrono::milliseconds>(performance_checking_id).count();
	std::cout << "Other timers: " << performance_time << std::endl;
}

void setupShaders() {

	cameraUBO = new uniformData(GL_UNIFORM_BUFFER, &camera.camera_data, (2 * 16) * sizeof(float) , "camera_data");
	cameraUBO->active = true;
	cameraUBO->binding_index = 0;
	int j = -1;
	for (int i = 0; i < 1544; i++) {
		heart_wave.offsetwave[i] = heartbeat[i] * 2;
		heart_wave.offsetwave[i] -= 1;
		int colourNum = (int) heartbeat[i] * 16777215;
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
	
	shader_variable_id_hashtable = ht_create(1024);
	ht_put(shader_variable_id_hashtable, "sphere_shader_position", 0);
	ht_put(shader_variable_id_hashtable, "sphere_shader_normal", 1);
	ht_put(shader_variable_id_hashtable, "sphere_shader_timeDiff", sphereShader.uniformTable[0]->uniformLocation);
	ht_put(shader_variable_id_hashtable, "sphere_shader_waveForm", sphereShader.uniformTable[1]->uniformLocation);
	ht_put(shader_variable_id_hashtable, "sphere_shader_alpha", sphereShader.uniformTable[2]->uniformLocation);
	ht_put(shader_variable_id_hashtable, "sphere_shader_clippy", sphereShader.uniformTable[3]->uniformLocation);

	unsigned int test = ht_get(shader_variable_id_hashtable, "sphere_shader_normal");

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
	entityShader.RegisterAttribute("tangent", 3);
	entityShader.RegisterAttribute("bitangent", 4);
	entityShader.RegisterUniform("translation");
	entityShader.RegisterUniform("normal_mapping");
	entityShader.RegisterTexture("diffuseTex", 0);
	entityShader.RegisterTexture("normalTex", 1);
	entityShader.registerUBO(cameraUBO);
	entityShader.registerUBO(lightingUBO);
	entityShader.registerUBO(clipUBO);
	entityShader.addShaderType(entityVPath, GL_VERTEX_SHADER);
	entityShader.addShaderType(entityFPath, GL_FRAGMENT_SHADER);
	entityShader.LoadShader();

	snakeShader.RegisterAttribute("position", 0);
	snakeShader.RegisterAttribute("normal", 1);
	snakeShader.RegisterAttribute("texCoords", 2);
	snakeShader.RegisterUniform("translation");
	snakeShader.RegisterUniform("time");
	snakeShader.registerUBO(cameraUBO);
	snakeShader.registerUBO(lightingUBO);
	snakeShader.registerUBO(clipUBO);
	snakeShader.addShaderType(snakeVPath, GL_VERTEX_SHADER);
	snakeShader.addShaderType(snakeFPath, GL_FRAGMENT_SHADER);
	snakeShader.LoadShader();

	

	guiShader.useShader();
	int zero = 0, one = 1, two = 2, three = 3;
	BoilerPlate::Shaders::Shader::loadInt2(guiShader.uniformTable[0]->uniformLocation, &zero);
	guiShader.stopShader();

	


	groundShader2.RegisterAttribute("position", 0);
	groundShader2.RegisterAttribute("height", 1);
	groundShader2.RegisterAttribute("textureCoords", 2);
	groundShader2.RegisterAttribute("normal", 3);
	groundShader2.RegisterAttribute("tangent", 4);
	groundShader2.RegisterAttribute("bitangent", 5);
	groundShader2.RegisterUniform("translation");
	groundShader2.RegisterUniform("tex");
	groundShader2.RegisterUniform("clippy");
	groundShader2.RegisterUniform("clipPlane");
	groundShader2.RegisterTexture("tex", 0);
	groundShader2.RegisterTexture("normalTex", 2);
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
	sData = new SaturationAdjustModule::SaturationData(1.3f);
	bloomData = new BloomEffectModule::BloomEffectData(1, 1, glm::vec2(Constants::DisplayProps.width, Constants::DisplayProps.height));
	postProcessPipeline->RegisterEffect(ContrastAdjust, cData);
	postProcessPipeline->RegisterEffect(BrightnessAdjust, bData);
	postProcessPipeline->RegisterEffect(SaturationAdjust, sData);
	postProcessPipeline->RegisterEffect(BloomEffect, bloomData);
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
	camera.roll = 3.14f / 2.0f;
	camera.yaw = 3.14f;

	player = new BoilerPlate::Physics::DynamicEntity();

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

void updateView() {
	updateViewProps();
	checkMouse(0);

	lighting.position[0] = camera.position.x;
	lighting.position[1] = camera.position.y;
	lighting.position[2] = camera.position.z;
	lighting.colour[0] = 1.0f;// (sin(glfwGetTime()) + 1.0f) / 2.0f * 0.8f + 0.2f;
	lighting.colour[1] = 1.0f;//(cos(glfwGetTime()) + 1.0f)/2.0f + 0.2f;
	lighting.colour[2] = 1.0f;// (sin(glfwGetTime()* 16.0f) + 1.0f) / 2.2f + 0.2f;

	glm::vec4 direction = camera.direction;
	float amount = 500.0f;
	if (forwardKey->isDownDown) {
		player->addForce(glm::vec3(direction.x * amount, direction.y *amount, direction.z * amount));
	}
	if (backKey->isDownDown) {
		player->addForce(glm::vec3(direction.x * -amount, direction.y *-amount, direction.z * -amount));
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

	std::chrono::nanoseconds physics_diff_nano = Timing::getTimeDiff<std::chrono::nanoseconds>(physics_timing_id);
	double physics_diff_double = physics_diff_nano.count() / 1000000000.0;

	terrain->updateState(camera.position);
	player->update((float)physics_diff_double);
	camera.position = player->position;



	if (player->position.y < terrain->getHeight(camera.position) + yOffset)
		player->position.y += (terrain->getHeight(player->position) + yOffset) - player->position.y;


	//roll is pitch - x
	//yaw is yaw -y


	camera.updateCam();

	if (camera.position.y < terrain->getHeight(camera.position) + yOffset)
		camera.position.y += (terrain->getHeight(camera.position) + yOffset) - camera.position.y;

	updateSnakes();
	timeytime += 0.15f;

}

void checkMouse(int action) {
	if (mouse->isDown(GLFW_MOUSE_BUTTON_LEFT)) {
		if (!isMouseHeld) {
			isMouseHeld = true;
			previousMousePos = mouse->getMousePosition();
			lastMouseClock = (float) clock();
			return;
		}
		float currentClock = (float) clock();
		float clockDiff = currentClock - lastMouseClock;

		glm::vec2 currentPos = mouse->getMousePosition();
		glm::vec2 posVector = currentPos - previousMousePos;
		clockDiff = clockDiff == 0 ? 1 : clockDiff;
		float magsPerClock = posVector.length() / clockDiff;
		previousMousePos = currentPos;
		lastMouseClock = currentClock;
		camera.moveBy(posVector * (magsPerClock * 0.01f));
	}
	if (!mouse->isDown(0) && isMouseHeld) {
		previousMousePos = glm::vec2(0, 0);
		isMouseHeld = false;
	}

}

/*
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
	renderer = Renderer();
	
	skybox = new Skybox(skyboxShader);
	skybox->genTextures(skyboxFiles2);
	renderer.addToRenderer(&skybox->renderMode);

	std::vector<BufferObject*> SphereData = BinaryLoader::readFile("./res/entities/sphereEntity.bin");
	SphereData[1]->update(floatProps, 0);
	SphereData[2]->update(floatProps, 1);
	SphereData[3]->update(floatProps, 2);

	sphere = new Entity();
	sphere->registerBufferObject(SphereData[1]);
	sphere->registerBufferObject(SphereData[3]);
	sphere->registerBufferObject(SphereData[2]);
	sphere->createEntity(SphereData[0]->data, SphereData[0]->size);
	sphereRenderer = RenderMode(GL_TRIANGLES, sphereShader);
	sphereRenderer.entityList.push_back(sphere);
	sphere->uniforms.push_back(uniformData(ht_get(shader_variable_id_hashtable, "sphere_shader_timeDiff"), 
			timeDiff, &BoilerPlate::Shaders::Shader::loadFloatArray));
	sphere->uniforms.push_back(uniformData(ht_get(shader_variable_id_hashtable, "sphere_shader_alpha"),
			&waveAlpha, &BoilerPlate::Shaders::Shader::loadFloat2));
	sphere->uniforms.push_back(uniformData(ht_get(shader_variable_id_hashtable, "sphere_shader_clippy"),
			&clippy, &BoilerPlate::Shaders::Shader::loadBool2));
	sphere->units.push_back(new BatchUnit());
	renderer.addToRenderer(&sphereRenderer);

	BinaryLoader::freeData(SphereData);

	ground_imp.load_ground_textures();


	TerrainGenerator::generateSeed();

	std::vector<BufferObject*> groundTest2 = BinaryLoader::readFile("./res/entities/groundA.bin");
	groundTest2[3]->update(floatProps, 2);

	terrain = new Terrain(256, 256, "./res/entities/groundA.bin", groundShader2);
	terrain->chunkEntity->registerBufferObject(groundTest2[3]);
	terrain->chunkEntity->textures.push_back(ground_imp.ground_colour_tex);
	terrain->chunkEntity->textures.push_back(ground_imp.ground_normal_tex);
	terrain->chunkEntity->uniforms.push_back(uniformData(groundShader2.uniformTable.at(2)->uniformLocation,
			&clippy, &BoilerPlate::Shaders::Shader::loadBool2));
	terrain->chunkEntity->uniforms.push_back(uniformData(groundShader2.uniformTable.at(3)->uniformLocation,
			&clipPlane, &BoilerPlate::Shaders::Shader::loadVec42));
	terrain->chunkEntity->createEntity(groundTest2[0]->data, groundTest2[0]->size);
	terrain->chunkEntity->active = false;
	terrain->chunkEntity->transformationLocation = groundShader2.uniformTable.at(0)->uniformLocation;
	terrain->clearMeshData();

	BinaryLoader::freeData(groundTest2);


	double start = clock();

	for(int i = -1; i < 1; i++){
		for(int j = -1; j < 1; j++){
			terrain->registerChunk(i, j);
			//terrain->generateChunk(i, j);
			
		}
	}

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

	/*
	scissorUnit = new BatchUnit();

	std::vector<Entity*> *scisEnt = ModelLoader::readModel("./res/entities/Scissors.obj",aiProcess_Triangulate | aiProcess_FlipUVs);
	scissors = (*scisEnt)[0];
	scissors->uniforms.push_back(uniformData(entityShader.uniformTable.at(0)->uniformLocation, &scissorUnit->transformationMatrix,
			BoilerPlate::Shaders::Shader::loadMat42));
	scissorRenderer = RenderMode(GL_TRIANGLES, entityShader);
	scissorRenderer.entityList.push_back(scissors);
	scissors->units.push_back(scissorUnit);
	renderer.addToRenderer(&scissorRenderer);
	*/


	Texture *mainTexture = new Texture();
	mainTexture->texID = postProcessPipeline->getOutputBuffer()->colour_attachments[0];
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

	std::vector<BufferObject*> snakeBOList = BinaryLoader::readFile("./res/entities/snake.bin");
	snakeBOList[1]->update(floatProps, 0);
	snakeBOList[2]->update(floatProps, 1);

	snake = new Entity();
	snake->registerBufferObject(snakeBOList[1]);
	snake->registerBufferObject(snakeBOList[2]);
	snake->createEntity(snakeBOList[0]->data, snakeBOList[0]->size );

	snake->uniforms.push_back(uniformData(snakeShader.uniformTable.at(1)->uniformLocation, &timeytime,
		BoilerPlate::Shaders::Shader::loadFloat2));
	snake->transformationLocation = snakeShader.uniformTable.at(0)->uniformLocation;
	snakeRenderer = RenderMode(GL_TRIANGLES, snakeShader);
	snakeRenderer.entityList.push_back(snake);
	snakes = std::vector<Snake*>(10);
	for (int i = 0; i < snakes.size(); i++) {
		snakes[i] = new Snake(cameraUBO);
		snake->units.push_back(snakes[i]->getUnit());
		renderer.addToRenderer(snakes[i]->getParticleSystem()->getRenderMode());
	}
	renderer.addToRenderer(&snakeRenderer);
	BinaryLoader::freeData(snakeBOList);

	ImageData barrelDiffuse = ImageLoader::loadRAW("./res/images/barrel.raw");
	ImageData barrelNormal = ImageLoader::loadRAW("./res/images/barrelNormal.raw");

	Texture *barrelDiffuseTexture = new Texture(barrelDiffuse, GL_TEXTURE_2D);
	barrelDiffuseTexture->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	barrelDiffuseTexture->registerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	barrelDiffuseTexture->registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	barrelDiffuseTexture->registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	barrelDiffuseTexture->textureBank = GL_TEXTURE0;
	barrelDiffuseTexture->updateTexture();
	Texture *barrelNormalTexture = new Texture(barrelNormal, GL_TEXTURE_2D);
	barrelNormalTexture->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	barrelNormalTexture->registerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	barrelNormalTexture->registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	barrelNormalTexture->registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	barrelNormalTexture->textureBank = GL_TEXTURE1;
	barrelNormalTexture->updateTexture();
	ImageLoader::freeData(barrelDiffuse);
	ImageLoader::freeData(barrelNormal);

	scissorUnit = new BatchUnit();
	std::vector<Entity*> *barrelEnt = ModelLoader::readModel("./res/entities/barrel.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	Entity * barrel = barrelEnt->at(0);
	scissorUnit->transformationMatrix = glm::translate(glm::vec3(0, 10, 0));
	barrel->uniforms.push_back(uniformData(entityShader.uniformTable.at(0)->uniformLocation, &scissorUnit->transformationMatrix,
		BoilerPlate::Shaders::Shader::loadMat42));
	bool *truee = new bool(true);
	barrel->uniforms.push_back(uniformData(entityShader.uniformTable.at(1)->uniformLocation, truee,
		BoilerPlate::Shaders::Shader::loadBool2));
	barrel->textures.push_back(barrelDiffuseTexture);
	barrel->textures.push_back(barrelNormalTexture);
	RenderMode *barrelRenderer = new RenderMode(GL_TRIANGLES, entityShader);
	barrelRenderer->entityList.push_back(barrel);
	barrel->units.push_back(scissorUnit);
	renderer.addToRenderer(barrelRenderer);
}

void cleanup() {
	delete cameraUBO;
	delete waveformUBO;
	sphere->cleanUp();
	delete sphere;
	remove("./temp");
	delete water;
}

void updateSnakes() {
	float timeDiff = Timing::getTimeDiff<std::chrono::nanoseconds>(snake_timing_id).count();
	timeDiff /= 1000000000.0f;
	
	for (int i = 0; i < snakes.size(); i++) {
		snakes[i]->updateSnake(timeDiff, timeytime);
	}
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

void splashScreen() {

	ImageData loadingImage = ImageLoader::loadPNG("./res/images/loading.png");
	Texture loadingTex = Texture(loadingImage, GL_TEXTURE_2D);
	loadingTex.registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	loadingTex.registerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	loadingTex.registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	loadingTex.registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	loadingTex.textureBank = GL_TEXTURE0;
	loadingTex.updateTexture();
	ImageLoader::freeData(loadingImage);

	float loadingVerts[8] = {
		-1, -1,
		-1, 1,
		1, -1,
		1, 1
	};

	float loadingTexCoords[8] = {
		0, 1.0f,
		0, 0,
		1, 1,
		1, 0
	};

	Entity loadingEntity = Entity();
	BufferObject* testVerts = new BufferObject(floatPropsTwoD, 0, 8, loadingVerts);
	BufferObject* testTexs = new BufferObject(floatPropsTwoD, 1, 8, loadingTexCoords);
	loadingEntity.registerBufferObject(testVerts);
	loadingEntity.registerBufferObject(testTexs);
	loadingEntity.textures.push_back(&loadingTex);
	loadingEntity.createEntity(testTexIndices, 6);
	loadingEntity.units.push_back(new BatchUnit());

	guiShader.RegisterAttribute("position", 0);
	guiShader.RegisterAttribute("texCoords", 1);
	guiShader.RegisterUniform("tex");
	guiShader.RegisterTexture("tex", 0);
	guiShader.addShaderType(guiVPath, GL_VERTEX_SHADER);
	guiShader.addShaderType(guiFPath, GL_FRAGMENT_SHADER);
	guiShader.LoadShader();

	Renderer loadingRenderer;
	RenderMode loadingRenderMode = RenderMode(GL_TRIANGLES, guiShader);
	loadingRenderMode.entityList.push_back(&loadingEntity);
	loadingRenderer.addToRenderer(&loadingRenderMode);

	loadingRenderer.clear();
	loadingRenderer.Render();

	glfwSwapBuffers(glfwProps.window);

	loadingEntity.cleanUp();
	loadingTex.deleteTexture();
}