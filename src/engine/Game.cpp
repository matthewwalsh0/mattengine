#include "Game.h"

#include "ColliderComponent.h"
#include "ColourComponent.h"
#include "DeleteComponent.h"
#include "LightComponent.h"
#include "Mesh.h"
#include "ModelComponent.h"
#include "OrthoCameraComponent.h"
#include "PhysicsComponent.h"
#include "ScriptComponent.h"
#include "SkyBoxComponent.h"
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

	m_imgui.onInit();
	onInit();

	float currentTime = glfwGetTime();
	float fpsCurrentTime = glfwGetTime();
	int frameCount = 0;

	m_framebuffer = new MattEngine::Framebuffer(640, 480);
	m_depthMap = new Framebuffer(1024, 1024, true, true);

	while (m_window.isOpen()) {
		float newTime = glfwGetTime();
		float deltaTime = newTime - currentTime;
		currentTime = newTime;

		float fpsDelta = newTime - fpsCurrentTime;

		if (fpsDelta > 1.0f) {
			m_fps = frameCount / fpsDelta;

			frameCount = 0;
			fpsCurrentTime = newTime;
		}

		onUpdate(deltaTime);

		shadowPass();
		renderPass();

		renderer.clear();

		m_imgui.onUpdate();
		m_window.update();

		frameCount += 1;
	}

	glfwTerminate();
}

void Game::onUpdate(float deltaTime) {
	Renderer& renderer = Renderer::getInstance();
	Scene& scene = *m_scene;
	Window& window = Window::getInstance();

	scene.onBeforeUpdate(renderer);

	for (auto& timer : Timer::Timers) {
		timer->onUpdate(deltaTime);
	}

	scene.getRegistry().view<PhysicsComponent, TransformComponent>().each(
		[&](const auto entity, PhysicsComponent& physics,
			TransformComponent& transform) {
			physics.OldPosition = transform.Position;
			transform.Position.x += physics.Velocity.x * deltaTime;
			transform.Position.y += physics.Velocity.y * deltaTime;
		});

	auto staticEntities =
		scene.getRegistry().view<ColliderComponent, TransformComponent>();

	scene.getRegistry()
		.view<ColliderComponent, TransformComponent, PhysicsComponent>()
		.each([&](const auto movingEntity, ColliderComponent& movingCollider,
				  TransformComponent& movingTransform,
				  PhysicsComponent& movingPhysics) {
			for (auto [staticEntity, staticCollider, staticTransform] :
				staticEntities.each()) {
				if (scene.createEntity(staticEntity)
						.template hasComponent<PhysicsComponent>())
					continue;

				CollisionType collisionType =
					Physics::isColliding(movingTransform.Position,
						movingTransform.Size, staticTransform.Position,
						staticTransform.Size, movingPhysics.OldPosition);

				if (collisionType == CollisionType::NONE)
					continue;

				Entity staticEntityFull = scene.createEntity(staticEntity);
				Entity movingEntityFull = scene.createEntity(movingEntity);

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

	scene.getRegistry().view<ScriptComponent>().each(
		[&](const auto entity, ScriptComponent& scriptComponent) {
			if (!scriptComponent.Script->Entity) {
				scriptComponent.Script->Entity = scene.createEntity(entity);
				scriptComponent.Script->onCreate();
			}

			scriptComponent.Script->onUpdate(deltaTime, renderer, window);
		});

	scene.getRegistry().view<const DeleteComponent>().each(
		[&](const auto entity, const DeleteComponent& text) {
			scene.getRegistry().destroy(entity);
		});

	m_camera.onUpdate(deltaTime);
	scene.onUpdate(deltaTime);
}

void Game::shadowPass() {
	Renderer& renderer = Renderer::getInstance();
	Scene& scene = *m_scene;

	m_depthMap->bind();

	renderer.setViewport({0.0f, 0.0f}, {1024.0f, 1024.0f});
	renderer.clear({1.0f, 1.0f, 1.0f});

	Entity light = m_scene->getEntity("Light");

	glm::vec3& lightPosition =
		light.getComponent<TransformComponent>().Position;
	glm::vec3& lightColour = light.getComponent<ColourComponent>().Colour;

	Light mainLight(lightPosition, lightColour);

	renderer.beginFrame(m_camera, mainLight);

	glCullFace(GL_FRONT);

	scene.getRegistry()
		.view<TransformComponent, ColourComponent, TextureComponent>()
		.each([&](auto rawEntity, TransformComponent& transform,
				  ColourComponent& colour, TextureComponent& texture) {
			DrawCubeRequest request;
			request.Position = transform.Position;
			request.Size = transform.Size;
			request.Rotation = transform.Rotation;
			request.DepthOnly = true;

			renderer.drawCube(request);
		});

	scene.getRegistry().view<TransformComponent, ModelComponent>().each(
		[&](auto rawEntity, TransformComponent& transform,
			ModelComponent& model) {
			DrawModelRequest request(model.Model);
			request.Position = transform.Position;
			request.Size = transform.Size;
			request.Rotation = transform.Rotation;
			request.DepthOnly = true;

			renderer.drawModel(request);
		});

	glCullFace(GL_BACK);

	m_depthMap->unbind();
}

void Game::renderPass() {
	Renderer& renderer = Renderer::getInstance();
	Scene& scene = *m_scene;

	m_framebuffer->bind();

	renderer.setViewport(
		{0.0f, 0.0f}, {m_framebuffer->getWidth(), m_framebuffer->getHeight()});

	renderer.clear();

	Texture(m_depthMap->getDepthTextureId()).bind(1);

	scene.getRegistry().view<SkyBoxComponent>().each(
		[&](SkyBoxComponent& skyBox) { renderer.drawSkybox(skyBox.CubeMap); });

	scene.getRegistry()
		.view<TransformComponent, ColourComponent, LightComponent>()
		.each([&](TransformComponent& transform, ColourComponent& colour,
				  LightComponent& light) {
			DrawLightRequest request;
			request.Position = transform.Position;
			request.Size = transform.Size;
			request.Rotation = transform.Rotation;
			request.Colour = colour.Colour;

			renderer.drawLight(request);
		});

	scene.getRegistry()
		.view<TransformComponent, ColourComponent, TextureComponent>()
		.each([&](auto rawEntity, TransformComponent& transform,
				  ColourComponent& colour, TextureComponent& texture) {
			DrawCubeRequest request;
			request.Position = transform.Position;
			request.Size = transform.Size;
			request.Rotation = transform.Rotation;
			request.Colour = colour.Colour;
			request.Texture = &texture.Texture;
			request.TileCount = texture.TileCount;

			renderer.drawCube(request);
		});

	scene.getRegistry()
		.view<TransformComponent, ColourComponent, ModelComponent>()
		.each([&](auto rawEntity, TransformComponent& transform,
				  ColourComponent& colour, ModelComponent& model) {
			DrawModelRequest request(model.Model);
			request.Position = transform.Position;
			request.Size = transform.Size;
			request.Rotation = transform.Rotation;
			request.Colour = colour.Colour;

			renderer.drawModel(request);
		});

	m_framebuffer->unbind();
}

} // namespace MattEngine
