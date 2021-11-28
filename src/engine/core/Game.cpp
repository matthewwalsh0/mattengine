#include "Game.h"

#include "Log.h"
#include "Mesh.h"
#include "ModelStore.h"
#include "PostProcessingLayer.h"
#include "TextureStore.h"
#include "Timer.h"

#include "AnimationComponent.h"
#include "ColourComponent.h"
#include "DeleteComponent.h"
#include "LightComponent.h"
#include "ModelComponent.h"
#include "PerspectiveCameraComponent.h"
#include "PlayerControllerComponent.h"
#include "PointLightComponent.h"
#include "PythonScriptComponent.h"
#include "RigidBodyComponent.h"
#include "ScriptComponent.h"
#include "SkyBoxComponent.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

#include <stdlib.h>
#include <time.h>

namespace MattEngine {

const float UPDATE_DELTA = 1.0f / 60;

Game::Game(Window& window) : m_window(window), m_title(window.getTitle()) {
	s_instance = this;

	m_scene = std::make_shared<Scene>();
	m_layers.emplace_back(new ShadowLayer());
	m_layers.emplace_back(new PostProcessingLayer());
}

void Game::resize(const glm::vec2& size) {
	Renderer& renderer = Renderer::getInstance();
	renderer.setViewport({0.0f, 0.0f}, {size.x, size.y});

	float aspectRatio = size.x / size.y;
	getCamera().setAspectRatio(aspectRatio);

	m_framebufferMultisampled->resize(size.x, size.y);
	m_framebuffer->resize(size.x, size.y);

	for (auto& layer : m_layers)
		(*layer).onResize(size.x, size.y);
}

void Game::start() {
	srand(time(NULL));

	MattEngine::Renderer renderer;

	renderer.init();
	m_physics.init();

	onInit();

	for (auto& layer : m_layers)
		(*layer).onInit();

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

		if (!m_firstResize) {
			const glm::vec2 windowSize = m_window.getSize();
			resize(windowSize);
			m_firstResize = true;
		}

		if (m_active) {
			onUpdate(deltaTime);

			for (auto& layer : m_layers)
				(*layer).onUpdate();
		} else {
			for (auto& layer : m_layers)
				(*layer).onInactive(deltaTime);
		}

		for (auto& layer : m_layers)
			(*layer).onBeforeRender();

		renderPass(getCamera());

		for (auto& layer : m_layers)
			(*layer).onAfterRender();

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

	scene.getRegistry().view<PythonScriptComponent>().each(
		[&](const auto entity, PythonScriptComponent& pythonScriptComponent) {
			if (!pythonScriptComponent.Script) {
				pythonScriptComponent.Script =
					PythonScript(pythonScriptComponent.Path);
			}

			pythonScriptComponent.Script->onUpdate(deltaTime);
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

	onCustomUpdate(deltaTime);

	scene.getRegistry().view<PlayerControllerComponent>().each(
		[&](auto rawEntity, PlayerControllerComponent& playerController) {
			if (!playerController.Controller.isInit()) {
				Entity wrappedEntity = scene.createEntity(rawEntity);
				playerController.Controller.init(wrappedEntity);
			}

			if (m_cameraChanged) {
				playerController.Controller.reset();
			}

			playerController.Controller.onUpdate(deltaTime);
		});

	scene.getRegistry()
		.view<TransformComponent, PerspectiveCameraComponent>()
		.each([&](auto rawEntity, TransformComponent& transform,
				  PerspectiveCameraComponent& perspectiveCamera) {
			glm::vec3 cameraDirection = glm::normalize(glm::rotate(
				perspectiveCamera.Rotation, glm::vec3(0.0f, 0.0f, 1.0f)));

			getCamera().setPosition(
				glm::vec3(transform.Position.x, transform.Position.y + 1.0f,
					transform.Position.z) +
				(cameraDirection * -5.0f));
			getCamera().setRotation(perspectiveCamera.Rotation);
		});

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

	scene.getRegistry().view<AnimationComponent>().each(
		[&](auto rawEntity, AnimationComponent& animation) {
			if (!animation.Started) {
				animation.Animator.playAnimation(*animation.Animation);
				animation.Started = true;
			}

			animation.Animator.onUpdate(deltaTime);
		});

	if (m_active) {
		getCamera().onUpdate(deltaTime);
	}

	m_cameraChanged = false;
}

void Game::renderPass(Camera& camera) {
	Renderer& renderer = Renderer::getInstance();
	Scene& scene = *m_scene;

	m_framebufferMultisampled->bind();

	std::vector<PointLight*> pointLights;

	scene.getRegistry().view<PointLightComponent>().each(
		[&](PointLightComponent& pointLight) {
			pointLights.push_back(&pointLight.Light);
		});

	renderer.beginFrame(camera, pointLights);

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

	scene.getRegistry().view<TransformComponent, ColourComponent>().each(
		[&](auto rawEntity, TransformComponent& transform,
			ColourComponent& colour) {
			Entity actualEntity = scene.createEntity(rawEntity);

			if (RenderPhysicsObjects &&
				actualEntity.hasComponent<RigidBodyComponent>()) {
				return;
			}

			DrawCubeRequest request;
			request.Position = transform.Position;
			request.Size = transform.Size;
			request.Rotation = transform.Rotation;
			request.Colour =
				!colour.Emissive
					? colour.Colour
					: (colour.Intensity * glm::vec3(1.0f)) + colour.Colour;

			if (actualEntity.hasComponent<TextureComponent>()) {
				TextureComponent& texture =
					actualEntity.getComponent<TextureComponent>();

				request.Texture =
					&TextureStore::getInstance().getTexture(texture.Path);

				request.TileCount = texture.UseTileSize
										? request.Size.x / texture.TileSize
										: texture.TileCount;
			}

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

	scene.getRegistry().view<PythonScriptComponent>().each(
		[&](const auto entity, PythonScriptComponent& pythonScriptComponent) {
			if (!pythonScriptComponent.Script) {
				pythonScriptComponent.Script =
					PythonScript(pythonScriptComponent.Path);
			}
		});

	if (RenderPhysicsObjects) {
		scene.getRegistry().view<RigidBodyComponent>().each(
			[&](const auto entity, RigidBodyComponent& rigidDynamic) {
				if (!rigidDynamic.Body)
					return;

				PxShape* shape = nullptr;

				PxTransform physicsTransform =
					rigidDynamic.Body->getGlobalPose();
				rigidDynamic.Body->getShapes(&shape, sizeof(shape));

				PxVec3 size = shape->getGeometry().box().halfExtents;

				DrawCubeRequest request;
				request.Position = {physicsTransform.p.x, physicsTransform.p.y,
					physicsTransform.p.z};
				request.Size = {size.x * 2, size.y * 2, size.z * 2};
				request.Rotation =
					glm::quat(physicsTransform.q.w, physicsTransform.q.x,
						physicsTransform.q.y, physicsTransform.q.z);
				request.Colour = {0.0f, 1.0f, 0.0f};

				renderer.drawCube(request);
			});

		scene.getRegistry().view<PlayerControllerComponent>().each(
			[&](const auto entity,
				PlayerControllerComponent& playerController) {
				DrawCubeRequest request;
				request.Position = playerController.Controller.getPosition();
				request.Size = playerController.Controller.getSize();
				request.Rotation = playerController.Controller.getRotation();
				request.Colour = {0.0f, 0.0f, 1.0f};

				renderer.drawCube(request);
			});
	}

	m_framebufferMultisampled->copyToScreen();
	m_framebufferMultisampled->copy(*m_framebuffer);
	m_framebufferMultisampled->unbind();
}

} // namespace MattEngine
