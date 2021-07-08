#include "EditorLayer.h"

#include "Game.h"

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
	Game& game = Game::getInstance();
	Window& window = Window::getInstance();

	if (ImGui::IsKeyPressed(GLFW_KEY_ENTER)) {
		window.setMouseEnabled(false);
		window.setFocused(true);
		game.setCamera(*m_gameCamera);
		game.play();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		window.setMouseEnabled(true);
		window.setFocused(false);
		game.pause();
		game.setCamera(m_editorCamera);
		m_gameViewport.reset();
	}

	if (game.isActive()) {
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	} else {
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
	}

	ImGui::DockSpaceOverViewport();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	m_menuBar.render();

	bool newEntity = m_entityList.render();
	Entity selectedEntity = m_entityList.getSelectedEntity();
	m_componentEditor.render(selectedEntity, newEntity);
	m_gameViewport.render();

	ImGui::PopStyleVar();
	ImGui::Begin("Performance");
	ImGui::Text("FPS: %.2f", game.getFPS());
	ImGui::End();

	m_engineSettings.render();
	m_fileBrowser.render();
}
} // namespace MattEngine