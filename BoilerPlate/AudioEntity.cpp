#include "AudioEntity.h"



AudioEntity::AudioEntity() {
	alGenSources(1, &ID);
	alSourcef(ID, AL_GAIN, 1);
	alSourcef(ID, AL_PITCH, 1);
	alSource3f(ID, AL_POSITION, 0, 0, 0);
	alSource3f(ID, AL_VELOCITY, 0, 0, 0);
}


AudioEntity::~AudioEntity() {
}

void AudioEntity::cleanUp() {
	alDeleteSources(1, &ID);
}

void AudioEntity::playSound(ALuint buffer) {
	alSourcei(ID, AL_BUFFER, buffer);
	alSourcePlay(ID);

}
