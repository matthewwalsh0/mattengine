#ifndef ORTHO_CAMERA_COMPONENT_H
#define ORTHO_CAMERA_COMPONENT_H

#include <glm/glm.hpp>

struct OrthoCameraComponent {
	glm::vec4 Bounds;
	glm::vec2 Planes;

	OrthoCameraComponent(const glm::vec4& bounds, const glm::vec2& planes)
		: Bounds(bounds), Planes(planes) {}

	OrthoCameraComponent(const OrthoCameraComponent& other)
		: Bounds(other.Bounds), Planes(other.Planes) {}
};

#endif