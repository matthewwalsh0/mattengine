#include "Renderer.h"

#include "Log.h"
#include "VertexArray.h"
#include "Vertices.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace MattEngine {

void Renderer::init() {
	s_instance = this;

	m_cube.reset(new VertexArray(Vertices::CUBE, 6 * 6, {}, 0,
		{{GL_FLOAT, 3}, {GL_FLOAT, 3}, {GL_FLOAT, 2}}));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_shader.bind();
}

void Renderer::onUpdate(float deltaTime) { m_camera.onUpdate(deltaTime); }

void Renderer::draw(RenderRequest& request) {
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		glm::vec3(request.Size.x, request.Size.y, request.Size.z));

	glm::mat4 translate = glm::translate(glm::mat4(1.0f),
		glm::vec3(request.Position.x, request.Position.y, request.Position.z));

	glm::mat4 rotate =
		glm::rotate(glm::mat4(1.0f), glm::radians(request.Rotation.x),
			glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(request.Rotation.y),
			glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(request.Rotation.z),
			glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 model = translate * rotate * scale;

	Shader* shader = &m_shader;

	if (request.Shader) {
		shader = request.Shader;
	}

	shader->bind();

	if (request.Texture) {
		request.Texture->bind();
	} else {
		m_defaultTexture.bind();
	}

	if (request.CubeMap) {
		request.CubeMap->bind();
	}

	if (request.VertexArray) {
		request.VertexArray->bind();
	} else {
		m_cube->bind();
	}

	glDepthMask(request.DepthMask ? GL_TRUE : GL_FALSE);

	shader->setVec3("u_Colour", request.Colour);
	shader->setMat4("u_View", m_camera.getView());
	shader->setVec3("u_viewPosition", m_camera.getPosition());
	shader->setMat4("u_Model", model);
	shader->setMat4("u_Projection", m_camera.getProjection());
	shader->setBool("u_IsLight", request.IsLight);
	shader->setInt("u_Texture", 0);
	shader->setInt("u_TileCount", request.TileCount);

	if (request.IsLight) {
		shader->setVec3("u_LightPosition", request.Position);
		shader->setVec3("u_LightColour", request.Colour);
	}

	if (request.VertexArray && request.VertexArray->IndexCount > 0) {
		glDrawElements(
			GL_TRIANGLES, request.VertexArray->IndexCount, GL_UNSIGNED_INT, 0);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
} // namespace MattEngine