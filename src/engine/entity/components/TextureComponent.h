#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include "Texture.h"

using namespace MattEngine;

struct TextureComponent {
	std::string Path = "assets/textures/missing.png";
	bool UseTileSize = false;
	unsigned int TileCount = 1;
	float TileSize = 1.0f;

	TextureComponent() {}
	TextureComponent(const std::string path) : Path(path) {}
	TextureComponent(const std::string path, unsigned int tileCount)
		: Path(path), TileCount(tileCount) {}
};

#endif