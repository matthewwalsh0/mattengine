#ifndef FLY_CAMERA_CONTROLLER_H
#define FLY_CAMERA_CONTROLLER_H

#include "MouseRotationCameraController.h"

#include <glm/gtx/quaternion.hpp>

namespace MattEngine {

class FlyCameraController {
public:
	void init(glm::vec3& position, glm::quat& rotation);
	void onUpdate(float deltaTime);

private:
	glm::vec3* m_position;
	glm::quat* m_rotation;
	float m_speed = 30.0f;
	MouseRotationCameraController m_rotationController;
};

} // namespace MattEngine

#endif
