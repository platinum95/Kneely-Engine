#include "LoadAudio.h"



bool isBigEndian() {
	int a = 1;
	return !((char*)&a)[0];
}

int convertToInt(char* buffer, int len) {
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i<len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i<len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}


LoadAudio::LoadAudio() {
}


LoadAudio::~LoadAudio() {
}

WAV_File LoadAudio::loadWav(const char* filename) {
	WAV_File output = WAV_File();
	char buffer[4];
	std::ifstream in(filename, std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0) {
		//return nullptr;
	}
	in.read(buffer, 4);
	in.read(buffer, 4);      //WAVE
	in.read(buffer, 4);      //fmt
	in.read(buffer, 4);      //16
	in.read(buffer, 2);      //1
	in.read(buffer, 2);
	output.channel = convertToInt(buffer, 2);
	in.read(buffer, 4);
	output.sampleRate = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	output.bps = convertToInt(buffer, 2);
	in.read(buffer, 4);      //data
	in.read(buffer, 4);
	output.size = convertToInt(buffer, 4);
	char* data = new char[output.size];
	in.read(data, output.size);
	output.data = data;
	unsigned int form;
	if (output.channel == 1) {
		if (output.bps == 8) {
			form = AL_FORMAT_MONO8;
		}
		else {
			form = AL_FORMAT_MONO16;
		}
	}
	else {
		if (output.bps == 8) {
			form = AL_FORMAT_STEREO8;
		}
		else {
			form = AL_FORMAT_STEREO16;
		}
	}
	output.sound_format = form;

	return output;
	
	
		
}

void LoadAudio::freeData(WAV_File in) {
	delete in.data;
}
