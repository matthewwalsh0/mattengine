#ifndef GAME_H
#define GAME_H

#include "ColliderComponent.h"
#include "Entity.h"
#include "PhysicsComponent.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "TransformComponent.h"
#include "Window.h"

#include <string>

namespace MattEngine {

class Game {
public:
	virtual void onInit(){};

public:
	Game(Window& window);
	void start();
	void onUpdate(float deltaTime, Renderer& renderer, Window& window);
	const float inline getFPS() const { return m_fps; }
	std::shared_ptr<Scene> simulateUpdate(
		std::shared_ptr<Scene> scene, float deltaTime);

	template <typename T, typename... Args> void loadScene(Args&&... args) {
		m_scene.reset((Scene*)new T(std::forward<Args>(args)...));
		Scene::setCurrent(m_scene);
		m_scene->onInit();
	}

public:
	inline static Game& getInstance() { return *s_instance; }

private:
	inline static Game* s_instance;

private:
	void onUpdate(std::shared_ptr<Scene> scene, float deltaTime,
		Renderer& renderer, Window& window);

	Window& m_window;
	float m_fps;
	std::string m_title;
	std::shared_ptr<Scene> m_scene;
	Shader m_shaderSkybox = Shader("assets/shaders/skybox.glsl");
};
} // namespace MattEngine

#endif
