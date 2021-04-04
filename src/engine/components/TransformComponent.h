#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec2 Position;
    glm::vec2 Size;

    TransformComponent(const glm::vec2& position, const glm::vec2& size)
        : Position(position), Size(size) {}

    TransformComponent(const TransformComponent& other)
        : Position(other.Position), Size(other.Size) {}
};

#endif