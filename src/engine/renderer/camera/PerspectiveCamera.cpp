#include "PerspectiveCamera.h"

#include "Window.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace MattEngine;

PerspectiveCamera::PerspectiveCamera() {
	invalidate();
	m_controller.init(m_position, m_rotation);
}

void PerspectiveCamera::onUpdate(float deltaTime) {
	m_controller.onUpdate(deltaTime);
	invalidate();
}

void PerspectiveCamera::invalidate() {
	glm::vec3 direction = glm::rotate(m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	m_forward = glm::normalize(direction);
	m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
	m_projection =
		glm::perspective(glm::radians(m_angle), m_aspectRatio, 0.1f, 1000.0f);
}