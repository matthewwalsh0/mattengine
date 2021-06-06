#ifndef MESH_H
#define MESH_H

#include "Texture.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

#include <optional>
#include <string>
#include <vector>

namespace MattEngine {

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexturePosition;
	int BoneIds[4];
	float BoneWeights[4];
};

class Mesh {
public:
	VertexArray VertexArray;
	std::optional<Texture> Texture;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::optional<MattEngine::Texture> texture);
};
} // namespace MattEngine

#endif