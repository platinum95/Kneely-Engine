#include "ImageLoader.h"
#include <time.h>
#include <iostream>
#include <fstream>

ImageLoader::ImageLoader() {
}


ImageLoader::~ImageLoader() {

}

ImageData ImageLoader::loadPNG(const char * filePath) {
	std::vector<unsigned char> imageData;
	unsigned width, height;
	unsigned error = lodepng::decode(imageData, width, height, filePath);
	
	unsigned char* data = new unsigned char[imageData.size()];
	memcpy(data, &imageData[0], imageData.size() * sizeof(unsigned char));
	ImageData output = {
		width,
		height,
		imageData.size() * sizeof(unsigned char),
		data,
		imageData
	};
	//Delete this stuff


	return output;
}

ImageData ImageLoader::loadRAW(const char * filePath) {
	FILE *inFile = fopen(filePath, "rb");
	RawHeader header;
	std::vector<unsigned char> imageData = std::vector<unsigned char>();
	fread(&header, sizeof(RawHeader), 1, inFile);
	unsigned char* data = new unsigned char[header.dataSize];
	fread(data, 1, header.dataSize, inFile);
	fclose(inFile);
	imageData.resize(header.dataSize);


	ImageData output = {
		header.width,
		header.height,
		header.dataSize,
		data,
		imageData
	};
	return output;
}

void ImageLoader::PNGtoRAW(const char * filePath, const char * outFilePath) {
	ImageData pngData = loadPNG(filePath);
	RawHeader fileHeader = {
		pngData.width,
		pngData.height,
		pngData.dataSize
	};

	FILE *outFile = fopen(outFilePath, "wb");
	fwrite(&fileHeader, 3 * sizeof(unsigned int), 1, outFile);
	fwrite(&pngData.imageData[0], pngData.dataSize, 1, outFile);
	fclose(outFile);
}

void ImageLoader::freeData(ImageData in) {
	in.imageData.clear();
	delete in.data;
}


ImageData::~ImageData() {
	if (data != NULL) {
	//	delete data;
	}
	if (imageData.size() > 0) {
		imageData.clear();
	}
}
