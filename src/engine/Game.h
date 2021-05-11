#ifndef GAME_H
#define GAME_H

#include "ColliderComponent.h"
#include "Entity.h"
#include "Framebuffer.h"
#include "ImGuiLayer.h"
#include "Layer.h"
#include "OrthoCamera.h"
#include "PhysicsComponent.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "TransformComponent.h"
#include "Window.h"

#include <string>
#include <vector>

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

	Framebuffer* getFramebuffer() { return m_framebuffer; }
	Framebuffer* getDepthMap() { return m_depthMap; }
	std::shared_ptr<Scene> getScene() { return m_scene; }
	float getFps() { return m_fps; }

private:
	void onUpdate(std::shared_ptr<Scene> scene, float deltaTime,
		Renderer& renderer, Window& window);

	void renderPass(Renderer& renderer, Scene& scene, Shader& shader,
		bool includeLights = true);

public:
	inline static Game& getInstance() { return *s_instance; }

private:
	inline static Game* s_instance;

private:
	Window& m_window;
	float m_fps;
	std::string m_title;
	std::shared_ptr<Scene> m_scene;
	Shader m_shader = Shader("assets/shaders/core.glsl");
	Shader m_shaderSkybox = Shader("assets/shaders/skybox.glsl");
	Shader m_shaderShadow = Shader("assets/shaders/shadow.glsl");
	Framebuffer* m_framebuffer = nullptr;
	Framebuffer* m_depthMap = nullptr;
	ImGuiLayer m_imgui;
};
} // namespace MattEngine

#endif
