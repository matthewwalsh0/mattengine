#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "Camera.h"

#include <glm/gtx/quaternion.hpp>

namespace MattEngine {

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera();
	const glm::mat4& getProjection() override { return m_projection; };
	const glm::mat4& getView() override { return m_view; };
	const glm::vec3& getPosition() override { return m_position; }
	const glm::quat getRotationQuaternion();
	void onUpdate(float deltaTime) override;

	void setControllerActive(bool active) { m_controllerActive = true; }

	const void setAspectRatio(float aspectRatio) override {
		m_aspectRatio = aspectRatio;
		invalidate();
	};

	void setPosition(const glm::vec3& position) {
		m_position = position;
		invalidate();
	};

	void setRotation(const glm::quat& rotation) {
		m_rotation = rotation;
		invalidate();
	};

	void enableMouse(bool enabled) {
		m_mouseEnabled = enabled;

		if (m_mouseEnabled) {
			m_mouseMoved = false;
		}
	}

private:
	void invalidate();

public:
	glm::vec3 m_position = {0.0f, 1.0f, 9.0f};
	glm::quat m_rotation =
		glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
	glm::vec3 m_forward = {0.0f, 0.0f, -1.0f};
	glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
	glm::mat4 m_projection;
	glm::mat4 m_view;
	float m_angle = 45.0f;
	float m_aspectRatio = 1.0f;
	float m_speed = 30.0f;
	float m_rotationSpeed = 90.0f;
	float m_lastMouseX = 0.0f;
	float m_lastMouseY = 0.0f;
	bool m_mouseMoved = false;
	bool m_mouseEnabled = false;
	bool m_controllerActive = false;
};

} // namespace MattEngine

#endif
