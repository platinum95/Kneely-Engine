

#include "AudioPlate.h"
#include <time.h>
#include <iostream>

AudioPlate::AudioPlate() {
	ALCdevice* device = alcOpenDevice(NULL);
	if (device == NULL) {
		return;
	}
	ALCcontext* context = alcCreateContext(device, NULL);
	if (context == NULL) {
		return;
	}
	alcMakeContextCurrent(context);
}

void AudioPlate::loadAudio(WAV_File* file) {
	WAVE_Format wave_format = file->format;
	
	file->buffer = new unsigned int;
	alGenBuffers(1, file->buffer);

	alBufferData(*file->buffer, file->sound_format, file->data,
		file->size, file->sampleRate);
}

void AudioPlate::setListenerData() {
	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListener3f(AL_ORIENTATION, 0, 0, -1);
}


