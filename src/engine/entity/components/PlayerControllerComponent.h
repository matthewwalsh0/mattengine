#ifndef PLAYER_CONTROLLER_COMPONENT_H
#define PLAYER_CONTROLLER_COMPONENT_H

#include "FPSPlayerController.h"

#include <glm/glm.hpp>

namespace MattEngine {

struct PlayerControllerComponent {
	FPSPlayerController Controller;

	PlayerControllerComponent() {}
};

} // namespace MattEngine
#endif