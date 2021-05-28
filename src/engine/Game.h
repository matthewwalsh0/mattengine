#ifndef GAME_H
#define GAME_H

#include "Entity.h"
#include "Framebuffer.h"
#include "ImGuiLayer.h"
#include "Layer.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"
#include "Physics.h"
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
	void resize(const glm::vec2& size);

	template <typename T, typename... Args> void loadScene(Args&&... args) {
		m_scene.reset((Scene*)new T(std::forward<Args>(args)...));
		Scene::setCurrent(m_scene);
		m_scene->onInit();
	}

	const float inline getFPS() const { return m_fps; }
	Framebuffer* getFramebuffer() { return m_framebuffer; }
	Framebuffer* getFramebufferMultisampled() {
		return m_framebufferMultisampled;
	}
	Framebuffer* getDepthMap() { return m_depthMap; }
	std::shared_ptr<Scene> getScene() { return m_scene; }
	PerspectiveCamera& getCamera() { return m_camera; }
	PerspectiveCamera& getEditorCamera() { return m_editorCamera; }
	Physics& getPhysics() { return m_physics; }

	void play() { m_active = true; }
	void pause() { m_active = false; }
	bool isActive() { return m_active; }
	void setFullscreen(bool fullscreen) { m_fullscreen = fullscreen; }
	bool isFullscreen() { return m_fullscreen; }

private:
	void onUpdate(float deltaTime);
	void shadowPass(Camera& camera);
	void renderPass();

public:
	inline static Game& getInstance() { return *s_instance; }

private:
	inline static Game* s_instance;

private:
	Window& m_window;
	float m_fps;
	std::string m_title;
	std::shared_ptr<Scene> m_scene;
	Framebuffer* m_framebuffer = nullptr;
	Framebuffer* m_framebufferMultisampled = nullptr;
	Framebuffer* m_depthMap = nullptr;
	ImGuiLayer m_imgui;
	PerspectiveCamera m_camera = PerspectiveCamera();
	PerspectiveCamera m_editorCamera = PerspectiveCamera();
	Physics m_physics;
	bool m_active = false;
	bool m_fullscreen = false;
};

} // namespace MattEngine

#endif
