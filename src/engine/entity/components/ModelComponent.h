#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include "Model.h"

struct ModelComponent {
	std::string Path = "";

	ModelComponent() {}
	ModelComponent(const std::string& path) : Path(path) {}
};

#endif