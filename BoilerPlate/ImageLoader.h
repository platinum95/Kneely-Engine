#pragma once
#include "lodepng.h"

const unsigned int RAW_HEADER_SIZE = 3 * sizeof(unsigned int);

struct ImageData {
	unsigned int width, height, dataSize;
	void* data;
//	std::vector<unsigned char> imageData;

	~ImageData();
};

struct RawHeader {
	unsigned int width, height, dataSize;
};

class ImageLoader {
public:
	ImageLoader();
	~ImageLoader();

	static ImageData loadPNG(const char* filePath);
	static ImageData loadRAW(const char* filePath);
	static void PNGtoRAW(const char* filePath, const char * outFilePath);
	static void freeData(ImageData);
};

