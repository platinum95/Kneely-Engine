#pragma once
#ifndef STRINGS_H
#define STRINGS_H
#include <string>

std::string skyboxFiles[] = { "./res/images/right.png", "./res/images/left.png", "./res/images/top.png",
"./res/images/bottom.png", "./res/images/back.png", "./res/images/front.png" };

std::string skyboxFiles2[] = { "./res/images/right.raw", "./res/images/left.raw", "./res/images/top.raw",
"./res/images/bottom.raw", "./res/images/back.raw", "./res/images/front.raw" };

std::string skyboxFiles3[] = { "./res/images/owen.raw", "./res/images/paula.raw", "./res/images/peter.raw",
"./res/images/frolov.raw", "./res/images/sully.raw", "./res/images/brian.raw" };

const char* snakeFile = "./res/entities/snake.obj";

const char* planeVertexPath = "./res/shaders/sphereVShader.glsl", *planeFragmentPath = "./res/shaders/sphereFShader.glsl";
const char* groundVertexPath = "./res/shaders/groundVShader.glsl", *groundFragmentPath = "./res/shaders/groundFShader.glsl";
const char *normalVertexPath = "./res/shaders/normalVShader.glsl", *normalFragmentPath = "./res/shaders/normalFShader.glsl", *normalGeometryPath = "./res/shaders/normalGShader.glsl";
const char *skyboxVPath = "./res/shaders/skyboxVShader.glsl", *skyboxFPath = "./res/shaders/skyboxFShader.glsl";
const char *entityVPath = "./res/shaders/entityVShader.glsl", *entityFPath = "./res/shaders/entityFShader.glsl";
const char *guiVPath = "./res/shaders/guiVShader.glsl", *guiFPath = "./res/shaders/guiFShader.glsl";
const char *waterVPath = "./res/shaders/waterVShader.glsl", *waterFPath = "./res/shaders/waterFShader.glsl";
const char *snakeVPath = "./res/shaders/snakeVShader.glsl", *snakeFPath = "./res/shaders/snakeFShader.glsl";

#endif