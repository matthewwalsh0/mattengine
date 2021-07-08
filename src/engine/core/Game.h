#ifndef GAME_H
#define GAME_H

#include "Entity.h"
#include "FPSPlayerController.h"
#include "Framebuffer.h"
#include "ImGuiLayer.h"
#include "Layer.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"
#include "Physics.h"
#include "PostProcessingLayer.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "ShadowLayer.h"
#include "TransformComponent.h"
#include "Window.h"

#include <string>
#include <vector>

namespace MattEngine {

class Game {
public:
	virtual void onInit(){};
	virtual void onCustomUpdate(float deltaTime){};

public:
	Game(Window& window);
	void start();
	void resize(const glm::vec2& size);

	const float inline getFPS() const { return m_fps; }
	Framebuffer* getFramebuffer() { return m_framebuffer; }
	Framebuffer* getFramebufferMultisampled() {
		return m_framebufferMultisampled;
	}
	Scene& getScene() { return m_scene; }
	PerspectiveCamera& getCamera() { return *m_camera; }
	void setCamera(PerspectiveCamera& camera) {
		m_camera = &camera;
		m_camera->reset();
		m_cameraChanged = true;
	}
	Physics& getPhysics() { return m_physics; }
	ShadowLayer& getShadows() { return ((ShadowLayer&)*(m_layers[0])); }
	PostProcessingLayer& getPostProcessing() {
		return ((PostProcessingLayer&)*(m_layers[1]));
	}

	void play() { m_active = true; }
	void pause() { m_active = false; }
	bool isActive() { return m_active; }
	void addLayer(Layer* layer) { m_layers.emplace_back(layer); }

private:
	void onUpdate(float deltaTime);
	void renderPass(Camera& camera);

public:
	inline static Game& getInstance() { return *s_instance; }

private:
	inline static Game* s_instance;

private:
	std::vector<std::unique_ptr<Layer>> m_layers;
	Window& m_window;
	float m_fps;
	std::string m_title;
	Scene m_scene;
	Framebuffer* m_framebuffer = nullptr;
	Framebuffer* m_framebufferMultisampled = nullptr;
	PerspectiveCamera m_defaultCamera = PerspectiveCamera();
	PerspectiveCamera* m_camera = &m_defaultCamera;
	Physics m_physics;
	bool m_active = true;
	bool m_cameraChanged = false;

public:
	bool RenderPhysicsObjects = false;
};

} // namespace MattEngine

#endif
