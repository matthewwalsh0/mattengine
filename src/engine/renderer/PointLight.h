#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>

namespace MattEngine {

struct PointLight {
	glm::vec3 Position = {0.0f, 5.0f, 0.0f};
	glm::vec3 Ambient = {0.1f, 0.1f, 0.1f};
	glm::vec3 Diffuse = {1.0f, 1.0f, 1.0f};
	glm::vec3 Specular = {1.0f, 1.0f, 1.0f};

	float Constant = 1.0f;
	float Linear = 0.14f;
	float Quadratic = 0.07f;
};
} // namespace MattEngine

#endif
