#include "EditorLayer.h"

#include "Game.h"
#include "PointLightComponent.h"

#include <glm/gtx/color_space.hpp>

namespace MattEngine {

void EditorLayer::onInit() {
	ImGuiLayer::onInit();

	Window::getInstance().setFocused(false);

	Game& game = Game::getInstance();
	game.pause();

	m_gameCamera = &game.getCamera();

	m_editorCamera.enableController(true);
	game.setCamera(m_editorCamera);
}

void EditorLayer::onResize(unsigned int width, unsigned int height) {
	float aspectRatio = width / (float)height;
	m_gameCamera->setAspectRatio(aspectRatio);
	m_editorCamera.setAspectRatio(aspectRatio);
}

void EditorLayer::onInactive(float deltaTime) {
	m_editorCamera.onUpdate(deltaTime);
}

void EditorLayer::onRender() {
	Renderer& renderer = Renderer::getInstance();
	Game& game = Game::getInstance();
	Scene& scene = game.getScene();
	Camera& camera = game.getCamera();

	if (m_engineSettings.RenderCameraBounds) {
		renderer.drawCameraBounds(*m_gameCamera, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (m_engineSettings.RenderShadowCameraBounds) {
		ShadowLayer& shadowLayer = ShadowLayer::getInstance();
		const glm::vec3 lightPosition = shadowLayer.getLightPosition();

		for (int depthMapIndex = 0; depthMapIndex < shadowLayer.DepthMapCount;
			 depthMapIndex++) {
			OrthoCamera shadowCamera = shadowLayer.getShadowCamera(
				*m_gameCamera, lightPosition, depthMapIndex);

			glm::vec3 cascadeIndicatorColour =
				shadowLayer.getCascadeColour(depthMapIndex);

			renderer.drawCameraBounds(shadowCamera, cascadeIndicatorColour);
		}
	}
}

void EditorLayer::onRenderInternal() {
	Game& game = Game::getInstance();
	Window& window = Window::getInstance();

	if (ImGui::IsKeyPressed(GLFW_KEY_ENTER)) {
		m_state = State::GAME;

		window.setMouseEnabled(false);
		window.setFocused(true);

		game.setCamera(*m_gameCamera);
		game.play();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_U)) {
		m_state = State::UPDATE;

		window.setMouseEnabled(true);
		window.setFocused(false);

		game.setCamera(m_editorCamera);
		game.play();

		m_gameViewport.reset();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		m_state = State::EDITOR;

		window.setMouseEnabled(true);
		window.setFocused(false);

		game.pause();
		game.setCamera(m_editorCamera);

		m_gameViewport.reset();
	}

	if (m_state == State::GAME) {
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	} else {
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
	}

	ImGui::DockSpaceOverViewport();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	m_menuBar.render();

	bool newEntity = m_entityList.render();
	std::optional<Entity>& selectedEntity = m_entityList.SelectedEntity;

	if (selectedEntity) {
		m_componentEditor.render(*selectedEntity, newEntity);
	}

	m_gameViewport.render(m_state != State::GAME);

	ImGui::PopStyleVar();
	ImGui::Begin("Performance");
	ImGui::Text("FPS: %.2f", game.getFPS());
	ImGui::End();

	m_engineSettings.render();
	m_fileBrowser.render();
}
} // namespace MattEngine