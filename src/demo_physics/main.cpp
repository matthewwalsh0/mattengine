#include "ColourComponent.h"
#include "FPSPlayerController.h"
#include "Game.h"
#include "LightComponent.h"
#include "ModelComponent.h"
#include "PerspectiveCameraComponent.h"
#include "RigidBodyComponent.h"
#include "SkyBoxComponent.h"
#include "TextureComponent.h"

using namespace MattEngine;

class DemoScene : public MattEngine::Scene {

private:
	FPSPlayerController m_playerController;

private:
	void createStack(
		const unsigned int stackSize = 20, const float boxSize = 1.0f) {
		for (int y = 0; y < stackSize; y++) {
			for (int x = 0; x < stackSize - y; x++) {
				MattEngine::Entity box = createEntity();
				box.addComponent<TagComponent>("Box");
				box.addComponent<TransformComponent>(
					glm::vec3((x * boxSize * 1.1 + (y * (boxSize / 2))) -
								  (stackSize * (boxSize / 2)),
						y * boxSize + (boxSize / 2), 0.0f),
					glm::vec3(boxSize, boxSize, boxSize));
				box.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
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

		MattEngine::Entity ball = createEntity();
		ball.addComponent<TagComponent>("Ball");
		ball.addComponent<TransformComponent>(
			position + glm::rotate(cameraRotation, {0.0f, 0.0f, 1.0f}),
			glm::vec3(size, size, size));
		ball.addComponent<ColourComponent>(glm::vec3(1.0f, 0.0f, 0.0f));
		ball.addComponent<TextureComponent>("assets/textures/wood.png");
		ball.addComponent<RigidBodyComponent>(
			glm::rotate(cameraRotation, glm::vec3(0.0f, 0.0f, 1.0f) * speed));
	}

	void reset() {
		for (auto& box : getEntities("Box")) {
			deleteEntity(box);
		}
		for (auto& box : getEntities("Ball")) {
			deleteEntity(box);
		}
	}

public:
	void onInit() override {
		MattEngine::Entity skybox = createEntity();
		skybox.addComponent<SkyBoxComponent>("assets/textures/skybox");

		MattEngine::Entity light = createEntity();
		light.addComponent<TagComponent>("Light");
		light.addComponent<TransformComponent>(
			glm::vec3(0.0f, 200.0f, 30.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		light.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		light.addComponent<LightComponent>();

		MattEngine::Entity floor = createEntity();
		floor.addComponent<TagComponent>("Floor");
		floor.addComponent<TransformComponent>(
			glm::vec3(0.0f, -0.15f, 0.0f), glm::vec3(100.0f, 0.1f, 100.0f));
		floor.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		floor.addComponent<TextureComponent>("assets/textures/floor.png", 50);

		MattEngine::Entity player = createEntity();
		player.addComponent<TagComponent>("Player");
		player.addComponent<TransformComponent>(
			glm::vec3(0.0f, 5.0f, 30.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		player.addComponent<PerspectiveCameraComponent>();

		createStack();

		m_playerController.init(player);
	}

	void onUpdate(float deltaTime) override {

		m_playerController.onUpdate(deltaTime);

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

class Demo : public MattEngine::Game {
public:
	Demo(MattEngine::Window& window) : Game(window) {}

	void onInit() override {}
};

int main(int argc, char** argv) {
	MattEngine::Window window(1280, 720, "MattEngine Demo");
	Demo(window).start();
	return 0;
}
