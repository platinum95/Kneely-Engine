#include "Renderer_TS.h"



Renderer_TS::Renderer_TS(){

	

}

bool Renderer_TS::isLocked() {
	return locked;
}


void Renderer_TS::lock() {
	renderer_holder = *renderer;

	locked = true;

}

void Renderer_TS::unlock() {
	*renderer = renderer_holder;
}



Armadillo::Graphics::Renderer* Renderer_TS::getRenderer() {
	if (locked)
		return &renderer_holder;

	return renderer;

}