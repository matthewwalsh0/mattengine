#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include "Model.h"

struct ModelComponent {
	MattEngine::Model Model;

	ModelComponent(MattEngine::Model& model) : Model(model) {}
};

#endif