#ifndef RIGID_BODY_COMPONENT_H
#define RIGID_BODY_COMPONENT_H

#include "PxPhysicsAPI.h"

struct RigidBodyComponent {
	PxRigidDynamic* Body = nullptr;
	glm::vec3 Velocity = {0.0f, 0.0f, 0.0f};

	RigidBodyComponent() {}
	RigidBodyComponent(const glm::vec3& velocity) : Velocity(velocity) {}
};

#endif
