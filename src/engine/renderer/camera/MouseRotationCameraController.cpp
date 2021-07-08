#include "MouseRotationCameraController.h"

#include "Game.h"
#include "Log.h"
#include "PerspectiveCameraComponent.h"
#include "Physics.h"

using namespace MattEngine;

void MouseRotationCameraController::init(glm::quat& rotation) {
	m_rotation = &rotation;

	*m_rotation = glm::quat(glm::vec3(glm::radians(m_rotationEulerDegrees.x),
		glm::radians(m_rotationEulerDegrees.y),
		glm::radians(m_rotationEulerDegrees.z)));
}

void MouseRotationCameraController::reset() {
	m_firstUpdate = true;
	Window::getInstance().resetMouseTracking();
}

void MouseRotationCameraController::onUpdate(float deltaTime) {
	Window& window = Window::getInstance();

	if (m_requireClick && !window.getMouseLeftButtonDown()) {
		m_firstUpdate = true;
		return;
	}

	if (!window.hasMouseMoved())
		return;

	const glm::vec2 mousePosition = window.getMousePosition();
	float xOffset = mousePosition.x - m_lastMousePosition.x;
	float yOffset = m_lastMousePosition.y - mousePosition.y;

	m_lastMousePosition = mousePosition;

	if (m_firstUpdate) {
		m_firstUpdate = false;
		return;
	}

	xOffset *= m_sensitivity;
	yOffset *= m_sensitivity;

	m_rotationEulerDegrees.x -= yOffset;
	m_rotationEulerDegrees.y -= xOffset;

	if (m_rotationEulerDegrees.x > 89.0f)
		m_rotationEulerDegrees.x = 89.0f;
	else if (m_rotationEulerDegrees.x < -89.0f)
		m_rotationEulerDegrees.x = -89.0f;

	if (m_rotationEulerDegrees.y > 180.0f)
		m_rotationEulerDegrees.y =
			-180.0f + (m_rotationEulerDegrees.y - 180.0f);
	else if (m_rotationEulerDegrees.y < -180.0f)
		m_rotationEulerDegrees.y = 180.0f + (m_rotationEulerDegrees.y + 180.0f);

	*m_rotation = glm::quat(glm::vec3(glm::radians(m_rotationEulerDegrees.x),
		glm::radians(m_rotationEulerDegrees.y),
		glm::radians(m_rotationEulerDegrees.z)));
}