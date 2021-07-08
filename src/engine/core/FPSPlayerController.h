#ifndef FPS_PLAYER_CONTROLLER_H
#define FPS_PLAYER_CONTROLLER_H

#include "PlayerController.h"

#include "MouseRotationCameraController.h"

#include <glm/gtx/quaternion.hpp>

namespace MattEngine {

class FPSPlayerController : PlayerController {
public:
	void init(Entity entity) override;
	void onUpdate(float deltaTime) override;
	const glm::vec3 getPosition();
	const glm::vec3 getSize();
	const glm::quat getRotation();

	bool isInit() { return m_entity; }
	void reset() { m_rotationController.reset(); }

private:
	Entity m_entity;
	float m_speed = 0.2f;
	glm::quat m_rotation;
	MouseRotationCameraController m_rotationController;
};

} // namespace MattEngine

#endif
