#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include "Texture.h"

using namespace MattEngine;

struct TextureComponent {
	Texture Texture;
	unsigned int TileCount = 1;

	TextureComponent(const std::string path) : Texture(path) {}
	TextureComponent(const std::string path, unsigned int tileCount)
		: Texture(path), TileCount(tileCount) {}
	TextureComponent(const MattEngine::Texture& texture) : Texture(texture) {}
};

#endif