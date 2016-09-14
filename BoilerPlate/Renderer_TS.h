#pragma once

#include "Renderer.h"

class Renderer_TS
{

	
public:

	Renderer_TS();
	Armadillo::Graphics::Renderer *renderer, renderer_holder;

	void lock();

	void unlock();

	Armadillo::Graphics::Renderer* getRenderer();

	Armadillo::Maths::Vector3 pos1, pos2;
	bool isLocked();



private:
	bool locked;

};

