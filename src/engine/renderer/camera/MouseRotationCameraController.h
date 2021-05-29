#ifndef MOUSE_ROTATION_CAMERA_CONTROLLER_H
#define MOUSE_ROTATION_CAMERA_CONTROLLER_H

#include <glm/glm.hpp>

namespace MattEngine {

class MouseRotationCameraController {
public:
	void init(glm::quat& rotation);
	void onUpdate(float deltaTime);

	void setRequireClick(bool requireClick) { m_requireClick = requireClick; }

private:
	glm::quat* m_rotation;
	float m_sensitivity = 0.3f;
	bool m_firstUpdate = true;
	bool m_requireClick = false;
	glm::vec2 m_lastMousePosition = {0.0f, 0.0f};
	glm::vec3 m_rotationEulerDegrees = {0.0f, 180.0f, 0.0f};
};

} // namespace MattEngine

#endif
