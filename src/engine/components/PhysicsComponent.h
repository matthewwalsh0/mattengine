#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <glm/glm.hpp>

struct PhysicsComponent {
    glm::vec2 Velocity = {0.0f, 0.0f};
    glm::vec2 OldPosition = {0.0f, 0.0f};

    PhysicsComponent() {}
    PhysicsComponent(const glm::vec2& velocity) : Velocity(velocity), OldPosition(glm::vec2(0.0f, 0.0f)) {}
    PhysicsComponent(const PhysicsComponent& other) : Velocity(other.Velocity), OldPosition(other.OldPosition) {}
};

#endif
