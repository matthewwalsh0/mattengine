#ifndef FLY_CAMERA_CONTROLLER_H
#define FLY_CAMERA_CONTROLLER_H

#include "MouseRotationCameraController.h"

#include <glm/gtx/quaternion.hpp>

namespace MattEngine {

class FlyCameraController {
public:
	void init(glm::vec3& position, glm::quat& rotation);
	void onUpdate(float deltaTime);

	void reset() { m_rotationController.reset(); }

private:
	glm::vec3* m_position;
	glm::quat* m_rotation;
	glm::vec3 m_limits = glm::vec3(100.0f);
	float m_speed = 30.0f;
	MouseRotationCameraController m_rotationController;
};

} // namespace MattEngine

#endif
