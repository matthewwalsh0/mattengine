#include "Renderer.h"

#include "Log.h"
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

	m_defaultBoneTransforms.reserve(100);

	for (int i = 0; i < 100; i++)
		m_defaultBoneTransforms.push_back(glm::mat4(1.0f));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::beginFrame(Camera& camera, Light& light) {
	m_shadowCamera.setPosition(light.getPosition());
	glm::mat4 lightSpaceMatrix = m_shadowCamera.getProjectionView();

	m_shader.bind();
	m_shader.setMat4("u_View", camera.getView());
	m_shader.setMat4("u_Projection", camera.getProjection());
	m_shader.setVec3("u_ViewPosition", camera.getPosition());
	m_shader.setVec3("u_Colour", glm::vec3(1.0f, 1.0f, 1.0f));
	m_shader.setInt("u_Texture", 0);
	m_shader.setInt("u_ShadowMap", 1);
	m_shader.setInt("u_TileCount", 1);
	m_shader.setMat4("u_LightSpaceMatrix", lightSpaceMatrix);
	m_shader.setVec3("u_LightPosition", light.getPosition());
	m_shader.setVec3("u_LightColour", light.getColour());

	for (int i = 0; i < 100; ++i)
		m_shader.setMat4("u_BoneTransforms[" + std::to_string(i) + "]",
			m_defaultBoneTransforms[i]);

	m_shaderShadow.bind();
	m_shaderShadow.setMat4("u_LightSpaceMatrix", lightSpaceMatrix);

	m_shaderSkybox.bind();
	m_shaderSkybox.setMat4("u_View", camera.getView());
	m_shaderSkybox.setMat4("u_Projection", camera.getProjection());
	m_shaderSkybox.setInt("u_Texture", 0);
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

} // namespace MattEngine