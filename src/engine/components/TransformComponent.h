#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent {
	glm::vec3 Position;
	glm::vec3 Size;
	float RotationAngle = 0.0f;
	glm::vec3 RotationAxis = {1.0f, 1.0f, 1.0f};

	TransformComponent(const glm::vec3& position, const glm::vec3& size)
		: Position(position), Size(size) {}
};

#endif