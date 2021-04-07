#ifndef RENDERER_H
#define RENDERER_H

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
	void drawQuad(const float x, const float y, const float width,
		const float height, const glm::vec3 colour) const;
	void drawQuadGradient(const float x, const float y, const float width,
		const float height, const glm::vec3 colourStart,
		const glm::vec3 colourEnd) const;
	void drawQuadTexture(const float x, const float y, const float width,
		const float height, const Texture& texture,
		const unsigned int tileCount = 1, float alpha = 1.0f) const;
	void drawText(const float x, const float y, const float height,
		const std::string& text, const glm::vec3 colour);
	void drawCube(RenderRequest& request);

public:
	inline static Renderer& getInstance() { return *s_instance; }

private:
	const unsigned int createShader(
		const std::string& vertexSrc, const std::string& fragmentSrc) const;
	void drawQuad(const float x, const float y, const float width,
		const float height, const unsigned int shaderId) const;

private:
	unsigned int m_shaderColour;
	unsigned int m_shaderTexture;
	unsigned int m_shaderText;
	FT_Face m_face;
	std::map<char, unsigned int> m_charTextures;
	std::map<char, float> m_charAspects;

private:
	inline static Renderer* s_instance;
};
} // namespace MattEngine

#endif