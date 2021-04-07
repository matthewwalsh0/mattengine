#ifndef RENDERER_H
#define RENDERER_H

#include "Shader.h"
#include "Texture.h"

#include <ft2build.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include FT_FREETYPE_H

#include <map>
#include <string>

namespace MattEngine {

class RenderRequest {
private:
	inline static glm::vec3 DEFAULT_COLOUR = glm::vec3(1.0f, 1.0f, 1.0f);
	inline static glm::vec3 DEFAULT = glm::vec3(0.0f, 0.0f, 0.0f);

public:
	glm::vec3 Position;
	glm::vec3 Size;
	glm::vec3 Rotation = DEFAULT;
	glm::vec3 Colour = DEFAULT_COLOUR;
	bool IsLight = false;

public:
	RenderRequest(glm::vec3& position, glm::vec3& size)
		: Position(position), Size(size) {}

	RenderRequest& withColour(glm::vec3& colour) {
		Colour = colour;
		return *this;
	}

	RenderRequest& withRotation(glm::vec3& rotation) {
		Rotation = rotation;
		return *this;
	}

	RenderRequest& withIsLight() {
		IsLight = true;
		return *this;
	}
};

class Renderer {
public:
	void init();
	void drawCube(RenderRequest& request);

public:
	inline static Renderer& getInstance() { return *s_instance; }

private:
	Shader m_shader = Shader("assets/shaders/core.glsl");

private:
	inline static Renderer* s_instance;
};
} // namespace MattEngine

#endif