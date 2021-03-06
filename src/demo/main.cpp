#include "AnimationComponent.h"
#include "ColourComponent.h"
#include "Game.h"
#include "LightComponent.h"
#include "ModelComponent.h"
#include "PointLightComponent.h"
#include "PythonScriptComponent.h"
#include "SkyBoxComponent.h"
#include "TagComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

#ifdef USE_EDITOR
#include "EditorGame.h"
#define PARENT_CLASS EditorGame
#else
#define PARENT_CLASS Game
#endif

class Demo : public MattEngine::PARENT_CLASS {
public:
	Demo(MattEngine::Window& window) : MattEngine::PARENT_CLASS(window) {}

	void onInit() override {
		PARENT_CLASS::onInit();

		MattEngine::Entity cube = getScene().createEntity();
		cube.addComponent<TagComponent>("Cube");
		cube.addComponent<TransformComponent>(
			glm::vec3(-1.0f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		cube.addComponent<ColourComponent>(glm::vec3(1.0f));
		cube.addComponent<TextureComponent>("assets/textures/wood.jpg");
		cube.addComponent<PythonScriptComponent>("assets/scripts/move.py");

		MattEngine::Entity model = getScene().createEntity();
		model.addComponent<TagComponent>("Dance");
		model.addComponent<TransformComponent>(
			glm::vec3(1.0f, 0.005f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
		model.addComponent<ColourComponent>(glm::vec3(1.0f, 1.0f, 1.0f));
		model.addComponent<ModelComponent>("assets/models/dance/dance.fbx");

		MattEngine::Entity light = getScene().createEntity();
		light.addComponent<TagComponent>("Light");

		PointLight pointLight;
		pointLight.Position.x = -0.01f;
		pointLight.Ambient = glm::vec3(1.0f);
		pointLight.Diffuse = glm::vec3(0.0f);
		pointLight.Specular = glm::vec3(0.0f);
		pointLight.Linear = 0.000f;
		pointLight.Quadratic = 0.000f;
		light.addComponent<PointLightComponent>(pointLight);

		MattEngine::Entity floor = getScene().createEntity();
		floor.addComponent<TagComponent>("Floor");
		floor.addComponent<TransformComponent>(
			glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(100.0f, 0.01f, 100.0f));
		floor.addComponent<ColourComponent>(glm::vec3(0.8f, 0.8f, 0.8f));
		floor.addComponent<TextureComponent>("assets/textures/floor.png", 50);

		MattEngine::Entity skybox = getScene().createEntity();
		skybox.addComponent<SkyBoxComponent>("assets/textures/skybox");

		getCamera().enableController(true);
	}
};

int main(int argc, char** argv) {
	MattEngine::Window window(1280, 720, "MattEngine Demo");
	Demo(window).start();
	return 0;
}