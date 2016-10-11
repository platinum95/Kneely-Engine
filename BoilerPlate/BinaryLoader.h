#pragma once

#ifndef BINARYLOADER_H
#define BINARYLOADER_H
#include <vector>
#include "Entity.h"
#include "ObjLoader.h"

/*Space for 15 individual attributes

{
char[10] name
short type
short dataTypeSize
uint offset (bytes) (from start of Data block)
uint size (bytes)
} x 15

{
data
}*/

typedef unsigned int uint;
struct informationBlock {
	char name[10];
	short dimension;
	short dataTypeSize;

	uint offset;
	uint size;

};

struct header {
	informationBlock headerInfo[15];
};

class BinaryLoader {
public:

	BinaryLoader();
	static std::vector<BufferObject*> readFile(const char* filepath);
	static std::vector<BufferObject*> readFile(const char* filepath, unsigned int* attributes, unsigned int count);
	void createFile(BoilerPlate::Properties::EntityProperties entProps, const char* outFile, BufferObject norm,  BufferObject tex);
	void createFile(const char* inFile, const char* outFile);
	void createFile(const char* filename, std::vector<BufferObject> bos);

	static void freeData(std::vector<BufferObject*>);
	static void freeData(BufferObject*);
	~BinaryLoader();

private:
	static const uint HEADER_SIZE = sizeof(header);
	static volatile long fileSize;
};

#endif
