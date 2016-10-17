#include "Ground.h"


Ground::Ground() {};

void Ground::load_ground_textures() {
	ImageData grass = ImageLoader::loadRAW("./res/images/grass/grass_colour.raw");
	ground_colour_tex = new Texture(grass, GL_TEXTURE_2D);
	ground_colour_tex->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ground_colour_tex->registerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ground_colour_tex->registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	ground_colour_tex->registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	ground_colour_tex->textureBank = GL_TEXTURE0;
	ground_colour_tex->updateTexture();
	ImageLoader::freeData(grass);
	ImageData grass_height = ImageLoader::loadRAW("./res/images/grass/grass_height.raw");
	ground_height_tex = new Texture(grass_height, GL_TEXTURE_2D);
	ground_height_tex->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ground_height_tex->registerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ground_height_tex->registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	ground_height_tex->registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	ground_height_tex->textureBank = GL_TEXTURE1;
	ground_height_tex->updateTexture();
	ImageLoader::freeData(grass_height);
	ImageData grass_normal = ImageLoader::loadRAW("./res/images/grass/grass_normal.raw");
	ground_normal_tex = new Texture(grass_normal, GL_TEXTURE_2D);
	ground_normal_tex->registerParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ground_normal_tex->registerParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ground_normal_tex->registerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	ground_normal_tex->registerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	ground_normal_tex->textureBank = GL_TEXTURE2;
	ground_normal_tex->updateTexture();
	ImageLoader::freeData(grass_normal);
}

