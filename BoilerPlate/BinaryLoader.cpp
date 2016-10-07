#include "BinaryLoader.h"
#include <fstream>
#include <iostream>
//0 No data
//1 1-D
//2 2-D
//3 3-D
//4 4-D
//5 5-D
//6 
//...
//9 unspecified

volatile long BinaryLoader::fileSize = 0;

BinaryLoader::BinaryLoader() {
}


std::vector<BufferObject*> BinaryLoader::readFile(const char * filepath) {
	std::ifstream infile(filepath, std::ios::in | std::ios::binary | std::ios::ate);
	long size = (long) infile.tellg();
	char* memblock = new char[size];
	infile.seekg(0, std::ios::beg);
	infile.read(memblock, size);
	infile.close();
	delete memblock;
	
	FILE* file;
	fopen_s(&file, filepath, "rb");
	if (file == NULL) {
		return std::vector<BufferObject*>();
	}
	std::vector<BufferObject*> outputlist = std::vector<BufferObject*>();
	header* headerBlock = new header;
	fseek(file, 0, SEEK_END);
	long fileSize;
	fileSize = size;
	unsigned char *fileData = new unsigned char[size];
	fclose(file);
	fopen_s(&file, filepath, "r");
	fread(fileData, 1, size, file);
	memcpy(headerBlock, fileData, HEADER_SIZE);
	
	unsigned char* dataStart = &fileData[HEADER_SIZE];

	for (int i = 0; i < 15; i++) {
		informationBlock currentBlock = headerBlock->headerInfo[i];
		if (currentBlock.dimension <= 0 || currentBlock.dimension > 100)
			continue;
		void * data = malloc(currentBlock.size);
		memcpy(data, &dataStart[currentBlock.offset], currentBlock.size);
		BufferObject *newOb = new BufferObject();
		newOb->data = data;
		newOb->size = currentBlock.size / currentBlock.dataTypeSize;
		newOb->unitSize = currentBlock.dataTypeSize;
		newOb->dimension = currentBlock.dimension;
		newOb->title = currentBlock.name;
		outputlist.push_back(newOb);
	}

	delete headerBlock;
	delete(fileData);
	fclose(file);
	return outputlist;
}

void BinaryLoader::createFile(BoilerPlate::Properties::EntityProperties entProps, const char * outFile, BufferObject normals, BufferObject tex) {
	FILE * outFileStream;
	fopen_s(&outFileStream, outFile, "wb");
	header headerBlock;
	uint indexSizeBytes = sizeof(uint) * entProps.indexSize;
	uint vertexSizeBytes = sizeof(float) * entProps.vertexSize;
	uint texSizeBytes = tex.size * tex.unitSize;

	headerBlock.headerInfo[0] = informationBlock{
		"Indices",
		4,
		4,
		0,
		indexSizeBytes
	};
	headerBlock.headerInfo[1] = informationBlock{
		"Vertices",
		3,
		4,
		indexSizeBytes,
		vertexSizeBytes
	};
	headerBlock.headerInfo[2] = informationBlock{
		"Normals",
		3,
		4,
		indexSizeBytes + vertexSizeBytes,
		vertexSizeBytes
	};
	headerBlock.headerInfo[3] = informationBlock{
		"TexCoord",
		2,
		4,
		indexSizeBytes + vertexSizeBytes + vertexSizeBytes,
		texSizeBytes
	};

	for (int i = 4; i < 15; i++) {
		headerBlock.headerInfo[i].dimension = 0;
	}
	fwrite(&headerBlock, sizeof(header), 1, outFileStream);
	fwrite(entProps.indices, 1, indexSizeBytes, outFileStream);
	fwrite(entProps.vertices, 1, vertexSizeBytes, outFileStream);
	fwrite(normals.data, 1, vertexSizeBytes, outFileStream);
	fwrite(tex.data, 1, texSizeBytes, outFileStream);

	fclose(outFileStream);
}

void BinaryLoader::createFile(const char * inFile, const char *outFile) {
	//Assuming inFile is an obj...
	BoilerPlate::Properties::EntityProperties entProps = ObjLoader::readObjProp3(inFile, 1);
	FILE * outFileStream;
	fopen_s(&outFileStream, outFile, "w");
	header headerBlock;
	headerBlock.headerInfo[0] = informationBlock{
		"Indices",
		3,
		4,
		0,
		entProps.indexSize
	};
	headerBlock.headerInfo[1] = informationBlock{
		"Vertices",
		3,
		4,
		0,
		entProps.indexSize
	};
	headerBlock.headerInfo[2] = informationBlock{
		"Normals",
		3,
		4,
		entProps.indexSize + entProps.vertexSize,
		entProps.vertexSize
	};

	fwrite(&headerBlock, sizeof(header), 1, outFileStream);
	fwrite(entProps.indices, 4, entProps.indexSize, outFileStream);
	fwrite(entProps.vertices, 4, entProps.vertexSize, outFileStream);
	fwrite(entProps.normals, 4, entProps.vertexSize, outFileStream);

	fclose(outFileStream);
}

void BinaryLoader::createFile(const char* outFile, std::vector<BufferObject> bos){
	FILE * outFileStream;
	fopen_s(&outFileStream, outFile, "w");
	header headerBlock;
	headerBlock.headerInfo[0] = informationBlock{
		"Heights",
		1,
		4,
		0,
		bos[0].size * 4
	};
	headerBlock.headerInfo[1] = informationBlock{
		"Normals",
		3,
		4,
		bos[0].size * 4,
		bos[1].size * 4
	};
	for (int i = 2; i < 15; i++) {
		headerBlock.headerInfo[i].dimension = 0;
	}
	fwrite(&headerBlock, sizeof(header), 1, outFileStream);
	fwrite(bos[0].data, 1, bos[0].size * 4, outFileStream);
	fwrite(bos[1].data, 1, bos[1].size * 4, outFileStream);
	fclose(outFileStream);
}

void BinaryLoader::freeData(std::vector<BufferObject*> list) {

	for (BufferObject *BO : list) {
		free(BO->data);
	}
}

void BinaryLoader::freeData(BufferObject* toFree) {

	free(toFree->data);
}

BinaryLoader::~BinaryLoader() {
}
