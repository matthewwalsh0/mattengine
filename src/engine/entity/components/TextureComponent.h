#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include "Texture.h"

using namespace MattEngine;

struct TextureComponent {
	std::string Path;
	unsigned int TileCount = 1;

	TextureComponent(const std::string path) : Path(path) {}
	TextureComponent(const std::string path, unsigned int tileCount)
		: Path(path), TileCount(tileCount) {}
};

#endif