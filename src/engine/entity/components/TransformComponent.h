#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct TransformComponent {
	glm::vec3 Position;
	glm::vec3 Size;
	glm::quat Rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	TransformComponent(const glm::vec3& position, const glm::vec3& size)
		: Position(position), Size(size) {}
};

#endif