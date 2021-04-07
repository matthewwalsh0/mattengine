#include "ColourComponent.h"
#include "Game.h"
#include "LightComponent.h"
#include "TagComponent.h"
#include "TransformComponent.h"

class DemoScene : public MattEngine::Scene {
private:
	float SPEED = 0.4f;

private:
	float m_deltaX = SPEED;

public:
	void onInit() override {
		MattEngine::Entity entity = createEntity();
		entity.addComponent<TagComponent>("Cube");
		entity.addComponent<TransformComponent>(
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		entity.addComponent<ColourComponent>(glm::vec3(1.0f, 0.0f, 0.0f));

		MattEngine::Entity light = createEntity();
		light.addComponent<TransformComponent>(
			glm::vec3(1.0f, 0.8f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		light.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		light.addComponent<LightComponent>();
	}

	void onUpdate(float deltaTime) override {
		TransformComponent& transform =
			getEntity("Cube").getComponent<TransformComponent>();
		transform.Rotation.y += 45.0f * deltaTime;
		transform.Rotation.x += 45.0f * deltaTime;
	}
};

class Demo : public MattEngine::Game {
public:
	Demo(MattEngine::Window& window) : Game(window) {}

	void onInit() override { loadScene<DemoScene>(); }
};

int main(int argc, char** argv) {
	MattEngine::Window window(640, 480, "MattEngine Demo");
	Demo(window).start();
	return 0;
}