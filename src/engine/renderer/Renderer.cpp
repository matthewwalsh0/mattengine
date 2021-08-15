#include "Renderer.h"

#include "Game.h"
#include "Log.h"
#include "PerspectiveCamera.h"
#include "VertexArray.h"
#include "Vertices.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace MattEngine {

namespace Utils {

static glm::mat4 calculateModel(const glm::vec3& position,
	const glm::vec3& size, const glm::quat& rotation) {
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), size);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotate = glm::mat4_cast(rotation);

	return translate * rotate * scale;
}

} // namespace Utils

void Renderer::init() {
	s_instance = this;

	m_cube.reset(new VertexArray(Vertices::CUBE, 6 * 6, {}, 0,
		{{GL_FLOAT, 3}, {GL_FLOAT, 3}, {GL_FLOAT, 2}}));

	m_quad.reset(new VertexArray(Vertices::QUAD, 6, {}, 0,
		{{GL_FLOAT, 3}, {GL_FLOAT, 3}, {GL_FLOAT, 2}}));

	m_defaultBoneTransforms.reserve(100);

	for (int i = 0; i < 100; i++)
		m_defaultBoneTransforms.push_back(glm::mat4(1.0f));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::beginFrame(
	Camera& camera, std::vector<PointLight*>& pointLights) {
	m_shader.bind();
	m_shader.setMat4("u_View", camera.getView());
	m_shader.setMat4("u_Projection", camera.getProjection());
	m_shader.setVec3("u_ViewPosition", camera.getPosition());
	m_shader.setVec3("u_Colour", glm::vec3(1.0f, 1.0f, 1.0f));
	m_shader.setInt("u_Texture", 0);
	m_shader.setInt("u_TileCount", 1);

	for (int i = 0; i < pointLights.size(); i++) {
		m_shader.setVec3(
			"u_PointLights", i, "position", pointLights[i]->Position);
		m_shader.setVec3(
			"u_PointLights", i, "ambient", pointLights[i]->Ambient);
		m_shader.setVec3(
			"u_PointLights", i, "diffuse", pointLights[i]->Diffuse);
		m_shader.setVec3(
			"u_PointLights", i, "specular", pointLights[i]->Specular);
		m_shader.setFloat(
			"u_PointLights", i, "constant", pointLights[i]->Constant);
		m_shader.setFloat("u_PointLights", i, "linear", pointLights[i]->Linear);
		m_shader.setFloat(
			"u_PointLights", i, "quadratic", pointLights[i]->Quadratic);
	}

	m_shader.setInt("u_PointLightCount", pointLights.size());

	for (int i = 0; i < 100; ++i)
		m_shader.setMat4("u_BoneTransforms[" + std::to_string(i) + "]",
			m_defaultBoneTransforms[i]);

	m_shaderSkybox.bind();
	m_shaderSkybox.setMat4("u_View", camera.getView());
	m_shaderSkybox.setMat4("u_Projection", camera.getProjection());
	m_shaderSkybox.setInt("u_Texture", 0);

	m_shaderRawTriangle.bind();
	m_shaderRawTriangle.setMat4("u_View", camera.getView());
	m_shaderRawTriangle.setMat4("u_Projection", camera.getProjection());
}

void Renderer::beginShadowFrame(Camera& camera, Camera& shadowCamera,
	int depthMapIndex, float depthMapFarPlane,
	glm::vec3 cascadeIndicatorColour) {
	glm::mat4 lightSpaceMatrix = shadowCamera.getProjectionView();

	m_shader.bind();
	m_shader.setInt(
		"u_DepthMap[" + std::to_string(depthMapIndex) + "]", depthMapIndex + 1);
	m_shader.setVec3(
		"u_DepthMapFarPlane[" + std::to_string(depthMapIndex) + "]",
		camera.getProjection() *
			glm::vec4(0.0f, 0.0f, -1 * depthMapFarPlane, 1.0f));
	m_shader.setMat4(
		"u_LightSpaceMatrix[" + std::to_string(depthMapIndex) + "]",
		lightSpaceMatrix);
	m_shader.setVec3("u_DepthMapColour[" + std::to_string(depthMapIndex) + "]",
		cascadeIndicatorColour);

	m_shaderShadow.bind();
	m_shaderShadow.setMat4("u_LightSpaceMatrix", lightSpaceMatrix);
}

void Renderer::clear(const glm::vec3& colour) {
	glClearColor(colour.x, colour.y, colour.z, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setViewport(const glm::vec2& start, const glm::vec2& size) {
	glViewport(start.x, start.y, size.x, size.y);
}

void Renderer::drawCube(DrawCubeRequest& request) {
	glm::mat4 model =
		Utils::calculateModel(request.Position, request.Size, request.Rotation);

	m_cube->bind();

	if (request.DepthOnly) {
		m_shaderShadow.bind();
		m_shaderShadow.setMat4("u_Model", model);
	} else {
		m_shader.bind();
		m_shader.setMat4("u_Model", model);
		m_shader.setVec3("u_Colour", request.Colour);
		m_shader.setInt("u_TileCount", request.TileCount);

		if (request.Texture) {
			request.Texture->bind();
		} else {
			m_defaultTexture.bind();
		}
	}

	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_shader.setInt("u_TileCount", 1);
}

void Renderer::drawModel(DrawModelRequest& request) {
	glm::mat4 model =
		Utils::calculateModel(request.Position, request.Size, request.Rotation);

	if (request.DepthOnly) {
		m_shaderShadow.bind();
		m_shaderShadow.setMat4("u_Model", model);
	} else {
		m_shader.bind();
		m_shader.setMat4("u_Model", model);
		m_shader.setVec3("u_Colour", request.Colour);

		if (request.BoneTransforms.size() > 0) {
			for (int i = 0; i < 100; ++i) {
				m_shader.setMat4("u_BoneTransforms[" + std::to_string(i) + "]",
					request.BoneTransforms[i]);
			}
		} else {
			for (int i = 0; i < 100; ++i) {
				m_shader.setMat4("u_BoneTransforms[" + std::to_string(i) + "]",
					m_defaultBoneTransforms[i]);
			}
		}
	}

	for (unsigned int i = 0; i < request.Model.Meshes.size(); i++) {
		Mesh& mesh = request.Model.Meshes[i];
		mesh.VertexArray.bind();

		if (mesh.Texture) {
			mesh.Texture->bind();
		} else {
			m_defaultTexture.bind();
		}

		glDrawElements(
			GL_TRIANGLES, mesh.VertexArray.IndexCount, GL_UNSIGNED_INT, 0);
	}
}

void Renderer::drawLight(DrawLightRequest& request) {
	glm::mat4 model =
		Utils::calculateModel(request.Position, request.Size, request.Rotation);

	m_cube->bind();
	m_defaultTexture.bind();

	m_shader.bind();
	m_shader.bind();
	m_shader.setMat4("u_Model", model);
	m_shader.setVec3("u_Colour", request.Colour);
	m_shader.setBool("u_IsLight", true);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_shader.setBool("u_IsLight", false);
}

void Renderer::drawSkybox(CubeMap& cubeMap) {
	m_shaderSkybox.bind();
	m_cube->bind();
	cubeMap.bind();

	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void Renderer::drawRawTriangles(
	const std::vector<glm::vec3>& positions, const glm::vec3& colour) {
	VertexArray vertexArray(
		positions.data(), positions.size(), {}, 0, {{GL_FLOAT, 3}});

	vertexArray.bind();
	m_shaderRawTriangle.bind();
	m_shaderRawTriangle.setVec3("u_Colour", colour);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, positions.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::drawCameraBounds(Camera& camera, const glm::vec3& colour) {
	const std::vector<glm::vec3>& cameraBounds = camera.getBounds();

	// clang-format off
	drawRawTriangles({
		// Front
		cameraBounds[0], cameraBounds[1], cameraBounds[2],
		cameraBounds[0], cameraBounds[2], cameraBounds[3],
		
		// Right
		cameraBounds[3], cameraBounds[2], cameraBounds[6],
		cameraBounds[3], cameraBounds[6], cameraBounds[7],

		// Back
		cameraBounds[4], cameraBounds[5], cameraBounds[6],
		cameraBounds[4], cameraBounds[6], cameraBounds[7],

		// Top
		cameraBounds[2], cameraBounds[1], cameraBounds[5],
		cameraBounds[2], cameraBounds[5], cameraBounds[6],

		// Left
		cameraBounds[0], cameraBounds[1], cameraBounds[5],
		cameraBounds[0], cameraBounds[5], cameraBounds[4],
		
		// Bottom
		cameraBounds[3], cameraBounds[0], cameraBounds[4],
		cameraBounds[3], cameraBounds[4], cameraBounds[7],
	}, colour);
	/// clang-format on
}

void Renderer::drawFramebuffer() {
	m_quad->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

} // namespace MattEngine