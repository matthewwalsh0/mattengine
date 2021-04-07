#ifndef PHYSICS_H
#define PHYSICS_H

#include "Texture.h"

#include "entt.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MattEngine {

enum CollisionType { HORIZONTAL, VERTICAL, NONE };

class Physics {
public:
	static CollisionType isColliding(const glm::vec2& firstPosition,
		const glm::vec2& firstSize, const glm::vec2& secondPosition,
		const glm::vec2& secondSize, const glm::vec2& oldFirstPosition);
};
} // namespace MattEngine

#endif