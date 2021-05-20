#ifndef PHYSICS_H
#define PHYSICS_H

#include "Entity.h"

#include "PxPhysicsAPI.h"
#include <glm/glm.hpp>

using namespace physx;

namespace MattEngine {

class Physics {

public:
	void init();
	void simulate(float deltaTime);
	PxRigidDynamic* createRigidDynamic(
		const glm::vec3& position, const glm::vec3& size);
	void setLinearVelocity(Entity& entity, const glm::vec3& velocity);

private:
	PxDefaultAllocator m_allocator;
	PxDefaultErrorCallback m_errorCallback;
	PxFoundation* m_foundation = NULL;
	PxPhysics* m_physics = NULL;
	PxDefaultCpuDispatcher* m_dispatcher = NULL;
	PxScene* m_scene = NULL;
	PxMaterial* m_material = NULL;
	PxPvd* m_pvd = NULL;
};

} // namespace MattEngine

#endif