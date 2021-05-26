#ifndef FPS_PLAYER_CONTROLLER_H
#define FPS_PLAYER_CONTROLLER_H

#include "PlayerController.h"

namespace MattEngine {

class FPSPlayerController : PlayerController {
public:
	void init(Entity entity) override;
	void onUpdate(float deltaTime) override;

private:
	Entity m_entity;
	float m_speed = 1.0f;
	float m_rotationSpeed = 90.0f;
	float m_lastMouseX = 0.0f;
	float m_lastMouseY = 0.0f;
	bool m_mouseMoved = false;
	bool m_mouseEnabled = true;
	glm::vec3 m_rotationEuler = {0.0f, 180.0f, 0.0f};
};

} // namespace MattEngine

#endif
