#ifndef SKYBOX_COMPONENT_H
#define SKYBOX_COMPONENT_H

#include "CubeMap.h"

using namespace MattEngine;

struct SkyBoxComponent {
	CubeMap CubeMap;

	SkyBoxComponent(const std::string directory) : CubeMap(directory) {}
};

#endif