#include "PerspectiveCamera.h"

#include "Window.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace MattEngine;

PerspectiveCamera::PerspectiveCamera() { invalidate(); }

const glm::quat PerspectiveCamera::getRotationQuaternion() {

	return m_rotation;
}

void PerspectiveCamera::onUpdate(float deltaTime) {
	if (!m_controllerActive)
		return;

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

	if (!m_mouseEnabled)
		return;

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

	m_rotation *=
		glm::angleAxis(glm::radians(-yOffset), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::angleAxis(glm::radians(-xOffset), glm::vec3(0.0f, 1.0f, 0.0f));

	invalidate();
}

void PerspectiveCamera::invalidate() {
	glm::vec3 direction = glm::rotate(m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	m_forward = glm::normalize(direction);
	m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
	m_projection =
		glm::perspective(glm::radians(m_angle), m_aspectRatio, 0.1f, 1000.0f);
}