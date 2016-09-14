#pragma once
#include <AL/al.h>

class AudioEntity {
public:
	AudioEntity();
	~AudioEntity();
	void cleanUp();
	void playSound(ALuint buffer);
	ALuint ID;

};

