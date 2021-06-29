#include "ImGuiLayer.h"

#include "AnimationComponent.h"
#include "ColourComponent.h"
#include "Dialogs.h"
#include "Framebuffer.h"
#include "Game.h"
#include "SceneExporter.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

namespace MattEngine {

void ImGuiLayer::onInit() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	(void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(
		Window::getInstance().getInternalWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ImGuiLayer::onAfterRender() {
	Game& game = Game::getInstance();
	Window& window = Window::getInstance();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (ImGui::IsKeyPressed(GLFW_KEY_ENTER)) {
		if (ImGui::IsKeyDown(GLFW_KEY_LEFT_SUPER)) {
			game.setFullscreen(true);
		}

		window.setMouseEnabled(false);
		window.setFocused(true);
		game.play();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		window.setMouseEnabled(true);
		window.setFocused(false);
		game.setFullscreen(false);
		game.pause();
		m_gameViewport.reset();
	}

	if (game.isFullscreen()) {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		return;
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

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace MattEngine