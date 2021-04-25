#ifndef RENDERER_H
#define RENDERER_H

#include "CubeMap.h"
#include "Mesh.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "Texture.h"

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
	Texture* Texture = nullptr;
	CubeMap* CubeMap = nullptr;
	Shader* Shader = nullptr;
	unsigned int TileCount = 1;
	VertexArray* VertexArray = nullptr;
	bool IsLight = false;
	bool DepthMask = true;

public:
	RenderRequest(glm::vec3& position, glm::vec3& size)
		: Position(position), Size(size) {}

	RenderRequest() {}
};

class Renderer {
public:
	void init();
	void onUpdate(float deltaTime);
	void draw(RenderRequest& request);

public:
	inline static Renderer& getInstance() { return *s_instance; }

private:
	std::shared_ptr<VertexArray> m_cube;
	Shader m_shader = Shader("assets/shaders/core.glsl");
	PerspectiveCamera m_camera = PerspectiveCamera();
	unsigned int m_defaultTextureData[1] = {0xFFFFFF};
	Texture m_defaultTexture = Texture(1, 1, 3, m_defaultTextureData);

private:
	inline static Renderer* s_instance;
};
} // namespace MattEngine

#endif