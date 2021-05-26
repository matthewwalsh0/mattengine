#include "Physics.h"

#include "RigidBodyComponent.h"

using namespace physx;

namespace MattEngine {

void Physics::init() {
	m_foundation =
		PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);

	m_pvd = PxCreatePvd(*m_foundation);

	PxPvdTransport* transport =
		PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

	m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_physics = PxCreatePhysics(
		PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), true, m_pvd);

	m_dispatcher = PxDefaultCpuDispatcherCreate(2);

	PxSceneDesc sceneDescription(m_physics->getTolerancesScale());
	sceneDescription.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDescription.cpuDispatcher = m_dispatcher;
	sceneDescription.filterShader = PxDefaultSimulationFilterShader;
	sceneDescription.flags = PxSceneFlag::eENABLE_STABILIZATION;
	m_scene = m_physics->createScene(sceneDescription);

	PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();

	if (pvdClient) {
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(
			PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_material = m_physics->createMaterial(1.0f, 1.0f, 0.5f);

	PxRigidStatic* groundPlane =
		PxCreatePlane(*m_physics, PxPlane(0, 1, 0, 0), *m_material);

	PxControllerManager* manager = PxCreateControllerManager(*m_scene);

	PxBoxControllerDesc playerControllerDesc;
	playerControllerDesc.material = m_material;

	m_playerController = manager->createController(playerControllerDesc);

	m_scene->addActor(*groundPlane);
}

void Physics::simulate(float deltaTime) {
	m_scene->simulate(deltaTime);
	m_scene->fetchResults(true);
}

PxRigidDynamic* Physics::createRigidDynamic(
	const glm::vec3& position, const glm::vec3& size) {
	PxShape* shape = m_physics->createShape(
		PxBoxGeometry(size.x / 2, size.y / 2, size.z / 2), *m_material);

	PxRigidDynamic* body = m_physics->createRigidDynamic(
		PxTransform(PxVec3(position.x, position.y, position.z)));

	body->attachShape(*shape);
	body->setStabilizationThreshold(0.05f);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);

	m_scene->addActor(*body);
	shape->release();

	return body;
}

void Physics::setLinearVelocity(Entity& entity, const glm::vec3& velocity) {
	RigidBodyComponent& component = entity.getComponent<RigidBodyComponent>();

	component.Body->setLinearVelocity(
		PxVec3(velocity.x, velocity.y, velocity.z));
}

} // namespace MattEngine