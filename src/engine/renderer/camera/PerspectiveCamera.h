#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "Camera.h"
#include "Scene.h"

#include "FlyCameraController.h"
#include "OrthoCamera.h"

#include <glm/gtx/quaternion.hpp>

#include <vector>

namespace MattEngine {

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera();
	PerspectiveCamera(float nearPlane, float farPlane);
	PerspectiveCamera(const PerspectiveCamera& existingCamera, float nearPlane,
		float farPlane);

	void onUpdate(float deltaTime) override;
	OrthoCamera getBoundingOrtho(const glm::vec3& viewDirection, Scene& scene);

	const glm::mat4& getProjection() override { return m_projection; };
	const glm::mat4& getView() override { return m_view; };
	const glm::vec3& getPosition() override { return m_position; }
	const glm::quat& getRotation() { return m_rotation; }
	void reset() { m_controller.reset(); }
	void enableController(bool enabled) { m_useController = enabled; }

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

private:
	void invalidate();

public:
	glm::vec3 m_position = {0.0f, 1.0f, 7.5f};
	glm::quat m_rotation =
		glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
	glm::vec3 m_forward = {0.0f, 0.0f, -1.0f};
	glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
	glm::mat4 m_projection;
	glm::mat4 m_view;
	float m_angle = 45.0f;
	float m_nearPlane = 0.3f;
	float m_farPlane = 500.0f;
	float m_aspectRatio = 1.0f;
	float m_speed = 30.0f;
	bool m_useController = false;
	FlyCameraController m_controller;
};

} // namespace MattEngine

#endif
