#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <string>
#include <vector>

namespace MattEngine {

class Model {
public:
	std::vector<Mesh> Meshes;

public:
	Model(const std::string& file, bool flip = false);
};
} // namespace MattEngine

#endif