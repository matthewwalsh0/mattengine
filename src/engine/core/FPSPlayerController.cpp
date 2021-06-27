#include "FPSPlayerController.h"

#include "Game.h"
#include "Log.h"
#include "ModelComponent.h"
#include "ModelStore.h"
#include "PerspectiveCameraComponent.h"
#include "Physics.h"

using namespace MattEngine;

const glm::vec3& FPSPlayerController::getPosition() {
	Physics& physics = Game::getInstance().getPhysics();
	PxController& playerController = physics.getPlayerController();
	PxVec3 physicsPosition = playerController.getActor()->getGlobalPose().p;

	return {physicsPosition.x, physicsPosition.y, physicsPosition.z};
}

const glm::quat& FPSPlayerController::getRotation() {
	Physics& physics = Game::getInstance().getPhysics();
	PxController& playerController = physics.getPlayerController();
	PxQuat rotation = playerController.getActor()->getGlobalPose().q;

	return glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
}

const glm::vec3& FPSPlayerController::getSize() {
	Physics& physics = Game::getInstance().getPhysics();
	PxController& playerController = physics.getPlayerController();
	PxShape* shape = nullptr;

	playerController.getActor()->getShapes(&shape, sizeof(shape));
	PxVec3 size = shape->getGeometry().box().halfExtents;

	return {size.x * 2, size.y * 2, size.z * 2};
}

void FPSPlayerController::init(Entity entity) {
	m_entity = entity;

	Physics& physics = Game::getInstance().getPhysics();
	PxController& playerController = physics.getPlayerController();
	TransformComponent& transform = m_entity.getComponent<TransformComponent>();
	PerspectiveCameraComponent& perspectiveCamera =
		m_entity.getComponent<PerspectiveCameraComponent>();

	glm::vec3 controllerPosition = transform.Position;

	if (m_entity.hasComponent<ModelComponent>()) {
		ModelComponent& model = m_entity.getComponent<ModelComponent>();
		Model& rawModel = ModelStore::getInstance().getModel(model.Path);

		glm::vec3 center = rawModel.getCenterOffset();

		controllerPosition += (center * transform.Size);
	}

	playerController.setPosition(PxExtendedVec3(
		controllerPosition.x, controllerPosition.y, controllerPosition.z));

	m_rotationController.init(m_rotation);
}

void FPSPlayerController::onUpdate(float deltaTime) {
	Window& window = Window::getInstance();
	Physics& physics = Game::getInstance().getPhysics();
	PxController& playerController = physics.getPlayerController();

	glm::vec3 up = {0.0f, 1.0f, 0.0f};

	TransformComponent& transform = m_entity.getComponent<TransformComponent>();
	PerspectiveCameraComponent& perspectiveCamera =
		m_entity.getComponent<PerspectiveCameraComponent>();

	glm::vec3 direction = glm::normalize(
		glm::rotate(perspectiveCamera.Rotation, glm::vec3(0.0f, 0.0f, 1.0f)));

	direction.y = 0.0f;

	PxControllerFilters filters;

	if (window.isKeyDown(GLFW_KEY_W)) {
		glm::vec3 disp = direction * m_speed;

		playerController.move(
			{disp.x, disp.y, disp.z}, 0.0f, deltaTime, filters);
	}

	if (Window::getInstance().isKeyDown(GLFW_KEY_S)) {
		glm::vec3 disp = -direction * m_speed;

		playerController.move(
			{disp.x, disp.y, disp.z}, 0.0f, deltaTime, filters);
	}

	if (Window::getInstance().isKeyDown(GLFW_KEY_A)) {
		glm::vec3 disp = glm::normalize(glm::cross(-direction, up)) * m_speed;
		playerController.move(
			{disp.x, disp.y, disp.z}, 0.0f, deltaTime, filters);
	}

	if (Window::getInstance().isKeyDown(GLFW_KEY_D)) {
		glm::vec3 disp = glm::normalize(glm::cross(direction, up)) * m_speed;
		playerController.move(
			{disp.x, disp.y, disp.z}, 0.0f, deltaTime, filters);
	}

	PxExtendedVec3 playerPosition = playerController.getPosition();
	transform.Position = {playerPosition.x, playerPosition.y, playerPosition.z};

	if (m_entity.hasComponent<ModelComponent>()) {
		ModelComponent& model = m_entity.getComponent<ModelComponent>();
		Model& rawModel = ModelStore::getInstance().getModel(model.Path);

		glm::vec3 center = rawModel.getCenterOffset();

		transform.Position -= (center * transform.Size);
	}

	transform.Rotation = m_rotation;

	m_rotationController.onUpdate(deltaTime);
	perspectiveCamera.Rotation = m_rotation;
}