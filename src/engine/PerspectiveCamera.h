#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "Camera.h"

namespace MattEngine {

class PerspectiveCamera : Camera {
public:
	PerspectiveCamera();
	const glm::mat4& getProjection() override { return m_projection; };
	const glm::mat4& getView() override { return m_view; };
	void onUpdate(float deltaTime) override;

private:
	void invalidate();

private:
	glm::vec3 m_position = {0.0f, 0.0f, 5.0f};
	glm::vec3 m_rotation = {0.0f, -90.0f, 0.0f};
	glm::vec3 m_forward = {0.0f, 0.0f, -1.0f};
	glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
	glm::mat4 m_projection;
	glm::mat4 m_view;
	float m_angle = 45.0f;
	float m_speed = 10.0f;
	float m_rotationSpeed = 90.0f;
};

} // namespace MattEngine

#endif
