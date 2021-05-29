#ifndef PERSPECTIVE_CAMERA_COMPONENT_H
#define PERSPECTIVE_CAMERA_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct PerspectiveCameraComponent {
	glm::quat Rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	PerspectiveCameraComponent() {}
};

#endif