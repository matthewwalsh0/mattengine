#ifndef MESH_H
#define MESH_H

#include "Texture.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace MattEngine {

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexturePosition;
};

class Mesh {
public:
	VertexArray VertexArray;
	Texture* Texture = nullptr;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		MattEngine::Texture* texture);
};
} // namespace MattEngine

#endif