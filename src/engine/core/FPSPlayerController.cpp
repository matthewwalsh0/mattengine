#include "FPSPlayerController.h"

#include "Game.h"
#include "Log.h"
#include "PerspectiveCameraComponent.h"
#include "Physics.h"

using namespace MattEngine;

void FPSPlayerController::init(Entity entity) {
	m_entity = entity;

	Physics& physics = Game::getInstance().getPhysics();
	PxController& playerController = physics.getPlayerController();
	TransformComponent& transform = m_entity.getComponent<TransformComponent>();
	PerspectiveCameraComponent& perspectiveCamera =
		m_entity.getComponent<PerspectiveCameraComponent>();

	playerController.setPosition(PxExtendedVec3(
		transform.Position.x, transform.Position.y, transform.Position.z));

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

	m_rotationController.onUpdate(deltaTime);
	perspectiveCamera.Rotation = m_rotation;
}