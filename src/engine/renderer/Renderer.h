#ifndef RENDERER_H
#define RENDERER_H

#include "CubeMap.h"
#include "Light.h"
#include "Mesh.h"
#include "Model.h"
#include "OrthoCamera.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/gtx/quaternion.hpp>

#include <vector>

namespace MattEngine {

class DrawCubeRequest {
public:
	glm::vec3 Position = {0.0f, 0.0f, 0.0f};
	glm::vec3 Size = {1.0f, 1.0f, 1.0f};
	glm::quat Rotation;
	glm::vec3 Colour = {1.0f, 1.0f, 1.0f};
	Texture* Texture = nullptr;
	unsigned int TileCount = 1;
	bool DepthOnly = false;

public:
	DrawCubeRequest() {}
};

class DrawModelRequest {
public:
	Model& Model;
	glm::vec3 Position = {0.0f, 0.0f, 0.0f};
	glm::vec3 Size = {1.0f, 1.0f, 1.0f};
	glm::quat Rotation;
	glm::vec3 Colour = {1.0f, 1.0f, 1.0f};
	std::vector<glm::mat4> BoneTransforms;
	bool DepthOnly = false;

public:
	DrawModelRequest(class Model& model) : Model(model) {}
};

class DrawLightRequest {
public:
	glm::vec3 Position = {0.0f, 0.0f, 0.0f};
	glm::vec3 Size = {1.0f, 1.0f, 1.0f};
	glm::quat Rotation;
	glm::vec3 Colour = {1.0f, 1.0f, 1.0f};

public:
	DrawLightRequest() {}
};

class Renderer {
public:
	void init();
	void beginFrame(Camera& camera, Light& light);
	void drawCube(DrawCubeRequest& request);
	void drawModel(DrawModelRequest& request);
	void drawLight(DrawLightRequest& request);
	void drawSkybox(CubeMap& cubeMap);
	void clear(const glm::vec3& colour = {0.0f, 0.0f, 0.0f});
	void setViewport(const glm::vec2& start, const glm::vec2& size);

public:
	inline static Renderer& getInstance() { return *s_instance; }

private:
	Shader m_shader = Shader("assets/shaders/core.glsl");
	Shader m_shaderSkybox = Shader("assets/shaders/skybox.glsl");
	Shader m_shaderShadow = Shader("assets/shaders/shadow.glsl");
	std::shared_ptr<VertexArray> m_cube;
	OrthoCamera m_shadowCamera = OrthoCamera({-5.0f, 5.0f, -5.0f, 5.0f},
		{0.1f, 50.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	unsigned int m_defaultTextureData[1] = {0xFFFFFF};
	Texture m_defaultTexture = Texture(1, 1, 3, m_defaultTextureData);
	std::vector<glm::mat4> m_defaultBoneTransforms;

private:
	inline static Renderer* s_instance;
};

} // namespace MattEngine

#endif