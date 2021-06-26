#include "Game.h"

#include "AnimationComponent.h"
#include "ColourComponent.h"
#include "DeleteComponent.h"
#include "LightComponent.h"
#include "Log.h"
#include "Mesh.h"
#include "ModelComponent.h"
#include "ModelStore.h"
#include "PerspectiveCameraComponent.h"
#include "RigidBodyComponent.h"
#include "ScriptComponent.h"
#include "SkyBoxComponent.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TextureStore.h"
#include "Timer.h"
#include "TransformComponent.h"

#include <stdlib.h>
#include <time.h>

namespace MattEngine {

const float UPDATE_DELTA = 1.0f / 60;

Game::Game(Window& window) : m_window(window), m_title(window.getTitle()) {
	s_instance = this;

	m_layers.emplace_back(new ShadowLayer());
	m_layers.emplace_back(new ImGuiLayer());
}

void Game::resize(const glm::vec2& size) {
	Renderer& renderer = Renderer::getInstance();
	renderer.setViewport({0.0f, 0.0f}, {size.x, size.y});

	float aspectRatio = size.x / size.y;
	m_camera.setAspectRatio(aspectRatio);
	m_editorCamera.setAspectRatio(aspectRatio);

	m_framebufferMultisampled->resize(size.x, size.y);
	m_framebuffer->resize(size.x, size.y);
}

void Game::start() {
	srand(time(NULL));

	MattEngine::Renderer renderer;

	renderer.init();
	m_physics.init();

	for (auto& layer : m_layers)
		(*layer).onInit();

	onInit();

	float currentTime = glfwGetTime();
	float fpsCurrentTime = glfwGetTime();
	int frameCount = 0;

	m_framebufferMultisampled =
		new MattEngine::Framebuffer(640, 480, true, true, 4);
	m_framebuffer = new MattEngine::Framebuffer(640, 480);

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

		const glm::vec2 windowSize = m_window.getSize();

		if (m_fullscreen &&
			m_framebufferMultisampled->getSize() != windowSize) {
			resize(windowSize);
		}

		Camera* camera = &m_camera;

		if (m_active) {
			onUpdate(deltaTime);
			m_camera.onUpdate(deltaTime);

			for (auto& layer : m_layers)
				(*layer).onUpdate();
		} else {
			m_editorCamera.onUpdate(deltaTime);
			camera = &m_editorCamera;
		}

		for (auto& layer : m_layers)
			(*layer).onBeforeRender();

		renderPass(*camera);

		if (!m_fullscreen) {
			renderer.clear();
		}

		for (auto& layer : m_layers)
			(*layer).onAfterRender();

		m_window.update();

		frameCount += 1;
	}

	glfwTerminate();
}

void Game::onUpdate(float deltaTime) {
	Renderer& renderer = Renderer::getInstance();
	Scene& scene = m_scene;
	Window& window = Window::getInstance();

	scene.onBeforeUpdate(renderer);

	for (auto& timer : Timer::Timers) {
		timer->onUpdate(deltaTime);
	}

	m_physics.simulate(deltaTime);

	scene.getRegistry().view<RigidBodyComponent, TransformComponent>().each(
		[&](const auto entity, RigidBodyComponent& rigidDynamic,
			TransformComponent& transform) {
			if (!rigidDynamic.Body) {
				rigidDynamic.Body = m_physics.createRigidDynamic(
					transform.Position, transform.Size);
			}

			if (rigidDynamic.Velocity != glm::vec3(0.0f, 0.0f, 0.0f)) {
				Entity wrappedEntity = scene.createEntity(entity);

				m_physics.setLinearVelocity(
					wrappedEntity, rigidDynamic.Velocity);

				rigidDynamic.Velocity = {0.0f, 0.0f, 0.0f};
			}

			PxTransform physicsTransform = rigidDynamic.Body->getGlobalPose();

			transform.Position.x = physicsTransform.p.x;
			transform.Position.y = physicsTransform.p.y;
			transform.Position.z = physicsTransform.p.z;
			transform.Rotation.w = physicsTransform.q.w;
			transform.Rotation.x = physicsTransform.q.x;
			transform.Rotation.y = physicsTransform.q.y;
			transform.Rotation.z = physicsTransform.q.z;
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
			Entity wrappedEntity = scene.createEntity(entity);

			if (wrappedEntity.hasComponent<RigidBodyComponent>()) {
				wrappedEntity.getComponent<RigidBodyComponent>()
					.Body->release();
			}

			scene.getRegistry().destroy(entity);
		});

	scene.onUpdate(deltaTime);

	scene.getRegistry().view<RigidBodyComponent, TransformComponent>().each(
		[&](const auto entity, RigidBodyComponent& rigidDynamic,
			TransformComponent& transform) {
			if (!rigidDynamic.Body) {
				rigidDynamic.Body = m_physics.createRigidDynamic(
					transform.Position, transform.Size);
			}

			if (rigidDynamic.Velocity != glm::vec3(0.0f, 0.0f, 0.0f)) {
				Entity wrappedEntity = scene.createEntity(entity);

				m_physics.setLinearVelocity(
					wrappedEntity, rigidDynamic.Velocity);

				rigidDynamic.Velocity = {0.0f, 0.0f, 0.0f};
			}

			PxTransform physicsTransform = rigidDynamic.Body->getGlobalPose();

			physicsTransform.p.x = transform.Position.x;
			physicsTransform.p.y = transform.Position.y;
			physicsTransform.p.z = transform.Position.z;
			physicsTransform.q.w = transform.Rotation.w;
			physicsTransform.q.x = transform.Rotation.x;
			physicsTransform.q.y = transform.Rotation.y;
			physicsTransform.q.z = transform.Rotation.z;

			rigidDynamic.Body->setGlobalPose(physicsTransform);
		});

	scene.getRegistry()
		.view<TransformComponent, PerspectiveCameraComponent>()
		.each([&](auto rawEntity, TransformComponent& transform,
				  PerspectiveCameraComponent& perspectiveCamera) {
			m_camera.setPosition(transform.Position);
			m_camera.setRotation(perspectiveCamera.Rotation);
		});

	scene.getRegistry().view<AnimationComponent>().each(
		[&](auto rawEntity, AnimationComponent& animation) {
			if (!animation.Started) {
				animation.Animator.playAnimation(*animation.Animation);
				animation.Started = true;
			}

			animation.Animator.onUpdate(deltaTime);
		});
}

void Game::renderPass(Camera& camera) {
	Renderer& renderer = Renderer::getInstance();
	Scene& scene = m_scene;

	m_framebufferMultisampled->bind();

	glm::vec3 lightPosition = {0.0f, 10.0f, 0.0f};
	glm::vec3 lightColour = {1.0f, 1.0f, 1.0f};
	Entity light = scene.getEntity("Light");

	if (light && light.hasComponent<TransformComponent>()) {
		lightPosition = light.getComponent<TransformComponent>().Position;
	}

	if (light && light.hasComponent<ColourComponent>()) {
		lightColour = light.getComponent<ColourComponent>().Colour;
	}

	Light mainLight(lightPosition, lightColour);

	renderer.beginFrame(camera, mainLight);

	renderer.setViewport(
		{0.0f, 0.0f}, {m_framebuffer->getWidth(), m_framebuffer->getHeight()});

	renderer.clear();

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
			request.Texture =
				&TextureStore::getInstance().getTexture(texture.Path);
			request.TileCount = texture.TileCount;

			renderer.drawCube(request);
		});

	scene.getRegistry()
		.view<TransformComponent, ColourComponent, ModelComponent>()
		.each([&](auto rawEntity, TransformComponent& transform,
				  ColourComponent& colour, ModelComponent& model) {
			Model& modelReference =
				ModelStore::getInstance().getModel(model.Path);

			DrawModelRequest request(modelReference);
			request.Position = transform.Position;
			request.Size = transform.Size;
			request.Rotation = transform.Rotation;
			request.Colour = colour.Colour;

			Entity actualEntity = scene.createEntity(rawEntity);

			if (actualEntity.hasComponent<AnimationComponent>()) {
				AnimationComponent& animation =
					actualEntity.getComponent<AnimationComponent>();

				request.BoneTransforms = animation.Animator.getBoneTransforms();
			}

			renderer.drawModel(request);
		});

	if (m_fullscreen) {
		m_framebufferMultisampled->copyToScreen();
	} else {
		m_framebufferMultisampled->copy(*m_framebuffer);
	}

	m_framebuffer->unbind();
}

} // namespace MattEngine
