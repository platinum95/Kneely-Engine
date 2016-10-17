#pragma once
#include "Texture.h"

class Ground {
public:
	Ground();
	void load_ground_textures();
	Texture *ground_colour_tex, *ground_height_tex, *ground_normal_tex;
};
