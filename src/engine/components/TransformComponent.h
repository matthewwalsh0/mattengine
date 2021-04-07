#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent {
	glm::vec3 Position;
	glm::vec3 Size;
	glm::vec3 Rotation;

	TransformComponent(const glm::vec3& position, const glm::vec3& size)
		: Position(position), Size(size) {}

	TransformComponent(const glm::vec3& position, const glm::vec3& size,
		const glm::vec3& rotation)
		: Position(position), Size(size), Rotation(rotation) {}

	TransformComponent(const TransformComponent& other)
		: Position(other.Position), Size(other.Size), Rotation(other.Rotation) {
	}
};

#endif