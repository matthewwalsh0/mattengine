#include "AnimationComponent.h"
#include "ColourComponent.h"
#include "Game.h"
#include "LightComponent.h"
#include "ModelComponent.h"
#include "ModelStore.h"
#include "PerspectiveCameraComponent.h"
#include "PlayerControllerComponent.h"
#include "PointLightComponent.h"
#include "RigidBodyComponent.h"
#include "SkyBoxComponent.h"
#include "TextureComponent.h"

#ifdef USE_EDITOR
#include "EditorGame.h"
#define PARENT_CLASS EditorGame
#else
#define PARENT_CLASS Game
#endif

using namespace MattEngine;

class Demo : public MattEngine::PARENT_CLASS {
public:
	Demo(MattEngine::Window& window) : PARENT_CLASS(window) {}

private:
	void createStack(
		const unsigned int stackSize = 20, const float boxSize = 1.0f) {
		for (int y = 0; y < stackSize; y++) {
			for (int x = 0; x < stackSize - y; x++) {
				MattEngine::Entity box = getScene().createEntity();
				box.addComponent<TagComponent>("Box");
				box.addComponent<TransformComponent>(
					glm::vec3((x * boxSize * 1.1 + (y * (boxSize / 2))) -
								  (stackSize * (boxSize / 2)),
						y * boxSize + (boxSize / 2), 0.0f),
					glm::vec3(boxSize, boxSize, boxSize));
				box.addComponent<ColourComponent>(glm::vec3(1.0f));
				box.addComponent<TextureComponent>("assets/textures/wood.jpg");
				box.addComponent<RigidBodyComponent>();
			}
		}
	}

	void fireBall(const float speed = 100.0f, const float size = 2.0f) {
		MattEngine::PerspectiveCamera& camera =
			(MattEngine::PerspectiveCamera&)Game::getInstance().getCamera();

		const glm::vec3& position = camera.getPosition();
		glm::quat cameraRotation = camera.getRotation();

		MattEngine::Entity ball = getScene().createEntity();
		ball.addComponent<TagComponent>("Ball");
		ball.addComponent<TransformComponent>(
			position + glm::rotate(cameraRotation, {0.0f, 0.0f, 5.0f}),
			glm::vec3(size, size, size));
		ball.addComponent<ColourComponent>(glm::vec3(1.0f, 0.0f, 0.0f));
		ball.addComponent<TextureComponent>("assets/textures/wood.jpg");
		ball.addComponent<RigidBodyComponent>(
			glm::rotate(cameraRotation, glm::vec3(0.0f, 0.0f, 1.0f) * speed));
	}

	void reset() {
		for (auto& box : getScene().getEntities("Box")) {
			getScene().deleteEntity(box);
		}
		for (auto& box : getScene().getEntities("Ball")) {
			getScene().deleteEntity(box);
		}
	}

public:
	void onInit() override {
		MattEngine::PARENT_CLASS::onInit();

		MattEngine::Entity skybox = getScene().createEntity();
		skybox.addComponent<SkyBoxComponent>("assets/textures/skybox");

		MattEngine::Entity light = getScene().createEntity();
		light.addComponent<TagComponent>("Light");

		PointLight pointLight;
		pointLight.Position.x = -0.01f;
		pointLight.Position.z = 10.0f;
		pointLight.Ambient = glm::vec3(1.0f);
		pointLight.Diffuse = glm::vec3(0.0f);
		pointLight.Specular = glm::vec3(0.0f);
		pointLight.Linear = 0.000f;
		pointLight.Quadratic = 0.000f;

		light.addComponent<PointLightComponent>(pointLight);

		MattEngine::Entity floor = getScene().createEntity();
		floor.addComponent<TagComponent>("Floor");
		floor.addComponent<TransformComponent>(
			glm::vec3(0.0f, -0.15f, 0.0f), glm::vec3(100.0f, 0.1f, 100.0f));
		floor.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		floor.addComponent<TextureComponent>("assets/textures/floor.png", 50);

		MattEngine::Entity player = getScene().createEntity();
		player.addComponent<TagComponent>("Player");
		player.addComponent<TransformComponent>(
			glm::vec3(0.0f, -0.1f, 30.0f), glm::vec3(0.01f, 0.01f, 0.01f));
		player.addComponent<PerspectiveCameraComponent>();
		player.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		player.addComponent<ModelComponent>("assets/models/ninja_running.fbx");
		player.addComponent<AnimationComponent>(
			&ModelStore::getInstance()
				 .getModel("assets/models/ninja_running.fbx")
				 .Animations[0]);
		player.addComponent<PlayerControllerComponent>();

		createStack();
	}

	void onCustomUpdate(float deltaTime) override {
		Window& window = Window::getInstance();

		static bool down = false;

		if (window.isKeyDown(GLFW_KEY_C)) {
			if (!down) {
				reset();
			}
			down = true;
		} else if (window.isKeyDown(GLFW_KEY_B)) {
			if (!down) {
				createStack();
			}
			down = true;
		} else if (window.isKeyDown(GLFW_KEY_SPACE)) {
			if (!down) {
				fireBall();
			}
			down = true;
		} else {
			down = false;
		}
	}
};

int main(int argc, char** argv) {
	MattEngine::Window window(1280, 720, "MattEngine Demo");
	Demo(window).start();
	return 0;
}
