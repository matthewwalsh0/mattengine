#include "Physics.h"

namespace MattEngine {

    CollisionType Physics::isColliding(
        const glm::vec2& firstPosition, const glm::vec2& firstSize,
        const glm::vec2& secondPosition, const glm::vec2& secondSize,
        const glm::vec2& oldFirstPosition) {
        bool collission = firstPosition.x + firstSize.x > secondPosition.x && secondPosition.x + secondSize.x > firstPosition.x &&
            firstPosition.y + firstSize.y > secondPosition.y && secondPosition.y + secondSize.y > firstPosition.y;
        
        if(!collission) return CollisionType::NONE;

        // Left
        if(oldFirstPosition.x + firstSize.x < secondPosition.x && firstPosition.x + firstSize.x > secondPosition.x)
            return CollisionType::HORIZONTAL;

        // Right
        if(oldFirstPosition.x > secondPosition.x + secondSize.x && firstPosition.x < secondPosition.x + secondSize.x)
            return CollisionType::HORIZONTAL;
        
        // Top
        if(oldFirstPosition.y + firstSize.y < secondPosition.y && firstPosition.y + firstSize.y > secondPosition.y)
            return CollisionType::VERTICAL;

        // Bottom
        if(oldFirstPosition.y > secondPosition.y + secondSize.y && firstPosition.y < secondPosition.y + secondSize.y)
            return CollisionType::VERTICAL;

        return CollisionType::VERTICAL;
    }

}