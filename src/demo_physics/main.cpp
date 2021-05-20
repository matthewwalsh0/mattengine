#include "ColourComponent.h"
#include "Game.h"
#include "LightComponent.h"
#include "ModelComponent.h"
#include "RigidBodyComponent.h"
#include "SkyBoxComponent.h"
#include "TextureComponent.h"

using namespace MattEngine;

class DemoScene : public MattEngine::Scene {

private:
	MattEngine::Texture m_texture =
		MattEngine::Texture("assets/textures/wood.jpg");

	MattEngine::Texture m_textureFloor =
		MattEngine::Texture("assets/textures/floor.png");

private:
	void createStack() {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20 - i; j++) {
				MattEngine::Entity box = createEntity();
				box.addComponent<TagComponent>("Box");
				box.addComponent<TransformComponent>(
					glm::vec3((j * 2) - (20 - i), i * 2 + 1, 0.0f) * 0.5f,
					glm::vec3(1.0f, 1.0f, 1.0f));
				box.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
				box.addComponent<TextureComponent>(m_texture);
				box.addComponent<RigidBodyComponent>();
			}
		}
	}

	void fireBall() {
		MattEngine::PerspectiveCamera& camera =
			(MattEngine::PerspectiveCamera&)Game::getInstance().getCamera();

		const glm::vec3& position = camera.getPosition();
		glm::quat cameraRotation = camera.getRotationQuaternion();

		MattEngine::Entity ball = createEntity();
		ball.addComponent<TagComponent>("Ball");
		ball.addComponent<TransformComponent>(
			position, glm::vec3(5.0f, 5.0f, 5.0f));
		ball.addComponent<ColourComponent>(glm::vec3(1.0f, 0.0f, 0.0f));
		ball.addComponent<TextureComponent>(m_texture);
		ball.addComponent<RigidBodyComponent>(
			glm::rotate(cameraRotation, glm::vec3(0.0f, 0.0f, -1.0f) * 200.0f));
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
		floor.addComponent<TextureComponent>(m_textureFloor, 50);

		createStack();
	}

	void onUpdate(float deltaTime) override {
		Window window = Window::getInstance();
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

	void onInit() override { loadScene<DemoScene>(); }
};

int main(int argc, char** argv) {
	MattEngine::Window window(1280, 720, "MattEngine Demo");
	Demo(window).start();
	return 0;
}
