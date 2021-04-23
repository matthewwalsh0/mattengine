#include "Game.h"

#include "ColliderComponent.h"
#include "ColourComponent.h"
#include "DeleteComponent.h"
#include "LightComponent.h"
#include "Mesh.h"
#include "ModelComponent.h"
#include "PhysicsComponent.h"
#include "ScriptComponent.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "Timer.h"
#include "TransformComponent.h"

#include <stdlib.h>
#include <time.h>

namespace MattEngine {

const float UPDATE_DELTA = 1.0f / 60;

Game::Game(Window& window) : m_window(window), m_title(window.getTitle()) {
	s_instance = this;
}

void Game::start() {
	srand(time(NULL));

	MattEngine::Renderer renderer;
	renderer.init();

	onInit();

	float currentTime = glfwGetTime();
	float fpsCurrentTime = glfwGetTime();
	int frameCount = 0;

	while (m_window.isOpen()) {
		float newTime = glfwGetTime();
		float deltaTime = newTime - currentTime;
		currentTime = newTime;

		float fpsDelta = newTime - fpsCurrentTime;

		if (fpsDelta > 1.0f) {
			m_fps = frameCount / fpsDelta;

			frameCount = 0;
			fpsCurrentTime = newTime;

			char newTitleOld[100];
			sprintf(newTitleOld, "%s (%.2f FPS)", m_title.c_str(), m_fps);
			m_window.setTitle(newTitleOld);
		}

		onUpdate(deltaTime, renderer, m_window);
		m_window.update();

		frameCount += 1;
	}

	glfwTerminate();
}

void Game::onUpdate(float deltaTime, Renderer& renderer, Window& window) {
	glClearColor(0.1f, 0.1f, 0.1f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	onUpdate(m_scene, deltaTime, renderer, window);
}

std::shared_ptr<Scene> Game::simulateUpdate(
	std::shared_ptr<Scene> scene, float deltaTime) {
	std::shared_ptr<Scene> simulationScene;

	if (!scene) {
		simulationScene = std::make_shared<Scene>();

		m_scene->getRegistry().each([&](auto entity) {
			Entity current = m_scene->createEntity(entity);
			Entity target = simulationScene->createEntity();

			if (current.hasComponent<TransformComponent>()) {
				target.addComponent<TransformComponent>(
					current.getComponent<TransformComponent>());
			}

			if (current.hasComponent<TagComponent>()) {
				target.addComponent<TagComponent>(
					current.getComponent<TagComponent>());
			}

			if (current.hasComponent<PhysicsComponent>()) {
				target.addComponent<PhysicsComponent>(
					current.getComponent<PhysicsComponent>());
			}

			if (current.hasComponent<ColliderComponent>()) {
				target.addComponent<ColliderComponent>(
					current.getComponent<ColliderComponent>());
			}
		});
	} else {
		simulationScene = scene;
	}

	Scene::setSimulation(true);
	Scene::setCurrent(simulationScene);

	onUpdate(simulationScene, deltaTime, Renderer::getInstance(),
		Window::getInstance());

	Scene::setCurrent(m_scene);
	Scene::setSimulation(false);

	return simulationScene;
}

void Game::onUpdate(std::shared_ptr<Scene> scene, float deltaTime,
	Renderer& renderer, Window& window) {
	scene->onBeforeUpdate(renderer);

	for (auto& timer : Timer::Timers) {
		timer->onUpdate(deltaTime);
	}

	scene->getRegistry().view<PhysicsComponent, TransformComponent>().each(
		[&](const auto entity, PhysicsComponent& physics,
			TransformComponent& transform) {
			physics.OldPosition = transform.Position;
			transform.Position.x += physics.Velocity.x * deltaTime;
			transform.Position.y += physics.Velocity.y * deltaTime;
		});

	auto staticEntities =
		scene->getRegistry().view<ColliderComponent, TransformComponent>();

	scene->getRegistry()
		.view<ColliderComponent, TransformComponent, PhysicsComponent>()
		.each([&](const auto movingEntity, ColliderComponent& movingCollider,
				  TransformComponent& movingTransform,
				  PhysicsComponent& movingPhysics) {
			for (auto [staticEntity, staticCollider, staticTransform] :
				staticEntities.each()) {
				if (scene->createEntity(staticEntity)
						.template hasComponent<PhysicsComponent>())
					continue;

				CollisionType collisionType =
					Physics::isColliding(movingTransform.Position,
						movingTransform.Size, staticTransform.Position,
						staticTransform.Size, movingPhysics.OldPosition);

				if (collisionType == CollisionType::NONE)
					continue;

				Entity staticEntityFull = scene->createEntity(staticEntity);
				Entity movingEntityFull = scene->createEntity(movingEntity);

				if (staticCollider.Handler) {
					staticCollider.Handler->onCollision(
						collisionType, staticEntityFull, movingEntityFull);
				}

				if (movingCollider.Handler) {
					movingCollider.Handler->onCollision(
						collisionType, staticEntityFull, movingEntityFull);
				}
			}
		});

	scene->getRegistry().view<ScriptComponent>().each(
		[&](const auto entity, ScriptComponent& scriptComponent) {
			if (!scriptComponent.Script->Entity) {
				scriptComponent.Script->Entity = scene->createEntity(entity);
				scriptComponent.Script->onCreate();
			}

			scriptComponent.Script->onUpdate(deltaTime, renderer, window);
		});

	scene->getRegistry()
		.view<TransformComponent, ColourComponent, LightComponent>()
		.each([&](TransformComponent& transform, ColourComponent& colour,
				  LightComponent& light) {
			RenderRequest request(transform.Position, transform.Size);
			request.Colour = colour.Colour;
			request.Rotation = transform.Rotation;
			request.IsLight = true;

			renderer.draw(request);
		});

	scene->getRegistry()
		.view<TransformComponent, ColourComponent, TextureComponent>()
		.each([&](auto rawEntity, TransformComponent& transform,
				  ColourComponent& colour, TextureComponent& texture) {
			RenderRequest request(transform.Position, transform.Size);
			request.Colour = colour.Colour;
			request.Rotation = transform.Rotation;
			request.Texture = &texture.Texture;

			renderer.draw(request);
		});

	scene->getRegistry()
		.view<TransformComponent, ColourComponent, ModelComponent>()
		.each([&](auto rawEntity, TransformComponent& transform,
				  ColourComponent& colour, ModelComponent& model) {
			RenderRequest request(transform.Position, transform.Size);
			request.Colour = colour.Colour;
			request.Rotation = transform.Rotation;

			for (unsigned int i = 0; i < model.Model.Meshes.size(); i++) {
				Mesh& mesh = model.Model.Meshes[i];
				request.VertexArray = &mesh.VertexArray;
				request.Texture = mesh.Texture;

				renderer.draw(request);
			}
		});

	scene->getRegistry().view<const DeleteComponent>().each(
		[&](const auto entity, const DeleteComponent& text) {
			scene->getRegistry().destroy(entity);
		});

	scene->onUpdate(deltaTime);
	renderer.onUpdate(deltaTime);
}
} // namespace MattEngine
