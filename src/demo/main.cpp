#include "AnimationComponent.h"
#include "ColourComponent.h"
#include "Game.h"
#include "LightComponent.h"
#include "ModelComponent.h"
#include "SkyBoxComponent.h"
#include "TagComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

class DemoScene : public MattEngine::Scene {
private:
	float SPEED = 0.4f;

private:
	float m_deltaX = SPEED;

	MattEngine::Model m_model = MattEngine::Model("assets/models/dog/mesh.fbx");

	MattEngine::Model m_model_2 =
		MattEngine::Model("assets/models/backpack/backpack.obj", true);

	MattEngine::Model m_model_3 =
		MattEngine::Model("assets/models/dance/scene.gltf", false);

	MattEngine::Texture m_texture =
		MattEngine::Texture("assets/textures/wood.jpg");

	MattEngine::Texture m_textureFloor =
		MattEngine::Texture("assets/textures/floor.png");

	float m_lightDirection = 1.0f;

public:
	void onInit() override {
		MattEngine::Entity cube = createEntity();
		cube.addComponent<TagComponent>("Cube");
		cube.addComponent<TransformComponent>(
			glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		cube.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		cube.addComponent<TextureComponent>(m_texture);

		MattEngine::Entity model = createEntity();
		model.addComponent<TagComponent>("Dog");
		model.addComponent<TransformComponent>(
			glm::vec3(-1.0f, -0.5f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));
		model.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		model.addComponent<ModelComponent>(m_model);

		MattEngine::Entity model2 = createEntity();
		model2.addComponent<TagComponent>("Backpack");
		model2.addComponent<TransformComponent>(
			glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f));
		model2.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		model2.addComponent<ModelComponent>(m_model_2);

		MattEngine::Entity model3 = createEntity();
		model3.addComponent<TagComponent>("Dance");
		model3.addComponent<TransformComponent>(
			glm::vec3(3.0f, -0.4f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		model3.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		model3.addComponent<ModelComponent>(m_model_3);
		model3.addComponent<AnimationComponent>(&m_model_3.Animations[0]);

		MattEngine::Entity light = createEntity();
		light.addComponent<TagComponent>("Light");
		light.addComponent<TransformComponent>(
			glm::vec3(-2.0f, 1.5f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		light.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		light.addComponent<LightComponent>();

		MattEngine::Entity floor = createEntity();
		floor.addComponent<TagComponent>("Floor");
		floor.addComponent<TransformComponent>(
			glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(100.0f, 0.01f, 100.0f));
		floor.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		floor.addComponent<TextureComponent>(m_textureFloor, 50);

		MattEngine::Entity skybox = createEntity();
		skybox.addComponent<SkyBoxComponent>("assets/textures/skybox");
	}

	void onUpdate(float deltaTime) override {
		TransformComponent& lightTransform =
			getEntity("Light").getComponent<TransformComponent>();

		lightTransform.Position.x += m_lightDirection * 0.5f * deltaTime;

		if (lightTransform.Position.x > 3.0f) {
			m_lightDirection = -1.0f;
		} else if (lightTransform.Position.x < -3.0f) {
			m_lightDirection = 1.0f;
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