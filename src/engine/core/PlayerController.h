#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Entity.h"

namespace MattEngine {

class PlayerController {
public:
	virtual void init(Entity entity) = 0;
	virtual void onUpdate(float deltaTime) = 0;
};

} // namespace MattEngine

#endif
