#include "FlyCameraController.h"

#include "Window.h"

using namespace MattEngine;

void FlyCameraController::init(glm::vec3& position, glm::quat& rotation) {
	m_position = &position;
	m_rotation = &rotation;

	m_rotationController.init(*m_rotation);
	m_rotationController.setRequireClick(true);
}

void FlyCameraController::onUpdate(float deltaTime) {
	Window& window = Window::getInstance();

	glm::vec3 direction =
		glm::normalize(glm::rotate(*m_rotation, glm::vec3(0.0f, 0.0f, 1.0f)));

	glm::vec3 up = {0.0f, 1.0f, 0.0f};

	if (window.isKeyDown(GLFW_KEY_W)) {
		*m_position += direction * deltaTime * m_speed;
	}

	if (window.isKeyDown(GLFW_KEY_S)) {
		*m_position -= direction * deltaTime * m_speed;
	}

	if (window.isKeyDown(GLFW_KEY_A)) {
		*m_position -=
			glm::normalize(glm::cross(direction, up)) * deltaTime * m_speed;
	}

	if (window.isKeyDown(GLFW_KEY_D)) {
		*m_position +=
			glm::normalize(glm::cross(direction, up)) * deltaTime * m_speed;
	}

	if (m_position->x > m_limits.x) {
		m_position->x = m_limits.x;
	} else if (m_position->x < -m_limits.x) {
		m_position->x = -m_limits.x;
	}

	if (m_position->y > m_limits.y) {
		m_position->y = m_limits.y;
	} else if (m_position->y < -m_limits.y) {
		m_position->y = -m_limits.y;
	}

	if (m_position->z > m_limits.z) {
		m_position->z = m_limits.z;
	} else if (m_position->z < -m_limits.z) {
		m_position->z = -m_limits.z;
	}

	m_rotationController.onUpdate(deltaTime);
}