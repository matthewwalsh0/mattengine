#include "PerspectiveCamera.h"

#include "Window.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace MattEngine;

PerspectiveCamera::PerspectiveCamera() {
	m_projection = glm::perspective(glm::radians(m_angle), 1.0f, 0.1f, 100.0f);
	invalidate();
}

void PerspectiveCamera::onUpdate(float deltaTime) {
	if (Window::getInstance().isKeyDown(GLFW_KEY_W)) {
		m_position += m_forward * deltaTime * m_speed;
		invalidate();
	}

	if (Window::getInstance().isKeyDown(GLFW_KEY_S)) {
		m_position -= m_forward * deltaTime * m_speed;
		invalidate();
	}

	if (Window::getInstance().isKeyDown(GLFW_KEY_Q)) {
		m_rotation.y -= deltaTime * m_rotationSpeed;
		invalidate();
	}

	if (Window::getInstance().isKeyDown(GLFW_KEY_E)) {
		m_rotation.y += deltaTime * m_rotationSpeed;
		invalidate();
	}

	if (Window::getInstance().isKeyDown(GLFW_KEY_A)) {
		m_position -=
			glm::normalize(glm::cross(m_forward, m_up)) * deltaTime * m_speed;
		invalidate();
	}

	if (Window::getInstance().isKeyDown(GLFW_KEY_D)) {
		m_position +=
			glm::normalize(glm::cross(m_forward, m_up)) * deltaTime * m_speed;
		invalidate();
	}

	Window& window = Window::getInstance();
	if (!window.MouseMoved)
		return;

	float xOffset = window.MouseX - m_lastMouseX;
	float yOffset = m_lastMouseY - window.MouseY;

	m_lastMouseX = window.MouseX;
	m_lastMouseY = window.MouseY;

	if (!m_mouseMoved) {
		m_mouseMoved = true;
		return;
	}

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	m_rotation.y += xOffset;
	m_rotation.x += yOffset;

	if (m_rotation.x > 89.0f)
		m_rotation.x = 89.0f;
	if (m_rotation.x < -89.0f)
		m_rotation.x = -89.0f;

	if (xOffset != 0 || yOffset != 0) {
		invalidate();
	}
}

void PerspectiveCamera::invalidate() {
	glm::vec3 direction;

	direction.x =
		cos(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));

	direction.y = sin(glm::radians(m_rotation.x));

	direction.z =
		sin(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));

	m_forward = glm::normalize(direction);
	m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
}