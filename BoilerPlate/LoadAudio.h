#pragma once
#ifndef LOADAUDIO_H
#define LOADAUDIO_H
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <fstream>

/*
* Struct that holds the RIFF data of the Wave file.
* The RIFF data is the meta data information that holds,
* the ID, size and format of the wave file
*/
struct RIFF_Header {
	char chunkID[4];
	long chunkSize;//size not including chunkSize or chunkID
	char format[4];
};

/*
* Struct to hold fmt subchunk data for WAVE files.
*/
struct WAVE_Format {
	char subChunkID[4];
	long subChunkSize;
	short audioFormat;
	short numChannels;
	long sampleRate;
	long byteRate;
	short blockAlign;
	short bitsPerSample;
};

/*
* Struct to hold the data of the wave file
*/
struct WAVE_Data_Info {
	char subChunkID[4]; //should contain the word data
	long subChunk2Size; //Stores the size of the data block
	
};

struct WAV_File {
	RIFF_Header header;
	WAVE_Format format;
	WAVE_Data_Info dataInfo;
	char* data;
	long dataSize;
	unsigned int sound_format;
	unsigned int *buffer;
	int channel, sampleRate, bps, size;
};




class LoadAudio {
public:
	LoadAudio();
	~LoadAudio();

	static WAV_File loadWav(const char* filePath);
	static void freeData(WAV_File);
};

#endif
