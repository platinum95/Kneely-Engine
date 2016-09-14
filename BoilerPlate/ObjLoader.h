#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Entity.h"
#include "Properties.h"
class ObjLoader {
public:
	ObjLoader();
	~ObjLoader();
	static BoilerPlate::Properties::EntityProperties readObjProp(const char* filePath, int offset);
	static BoilerPlate::Properties::EntityProperties readObjProp2(const char* filePath, int offset);
	static BoilerPlate::Properties::EntityProperties readObjProp3(const char* filePath, int offset);
};

#endif
