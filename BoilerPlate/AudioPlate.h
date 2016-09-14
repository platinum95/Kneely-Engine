#pragma once
#ifndef AUDIOPLATE_H
#define AUDIOPLATE_H

#include "LoadAudio.h"


class AudioPlate {

public:
	AudioPlate();

	void loadAudio(WAV_File*);
	void setListenerData();

};

#endif