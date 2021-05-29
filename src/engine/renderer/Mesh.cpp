#include "Mesh.h"

#include "VertexArray.h"

#include <glad/glad.h>

using namespace MattEngine;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
	MattEngine::Texture* texture)
	: VertexArray(MattEngine::VertexArray(vertices.data(), vertices.size(),
		  indices.data(), indices.size(),
		  {{GL_FLOAT, 3}, {GL_FLOAT, 3}, {GL_FLOAT, 2}})),
	  Texture(texture) {}