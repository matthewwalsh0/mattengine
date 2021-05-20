#include "ImGuiLayer.h"

#include "ColourComponent.h"
#include "Game.h"
#include "OrthoCameraComponent.h"
#include "TransformComponent.h"
#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MattEngine {

namespace ImGuiCustom {

static void EntityList(std::vector<Entity>& entities, Entity& selectedEntity) {
	ImGui::Begin("Entities");
	ImGui::BeginListBox("##EntityList",
		ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()));

	Game& game = Game::getInstance();

	for (auto& entity : game.getScene()->getAllEntities()) {
		if (ImGui::Selectable(entity.getComponent<TagComponent>().Tag.c_str(),
				selectedEntity == entity)) {
			selectedEntity = entity;
		}
	}

	ImGui::EndListBox();
	ImGui::End();
}

static void ComponentEditor(Entity& selectedEntity) {
	if (!selectedEntity)
		return;

	ImGui::Begin("Components");

	if (selectedEntity.hasComponent<TransformComponent>()) {
		TransformComponent& transform =
			selectedEntity.getComponent<TransformComponent>();

		if (ImGui::CollapsingHeader(
				"Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::SliderFloat3("Position", glm::value_ptr(transform.Position),
				-10.0f, 10.0f, "%.3f", ImGuiSliderFlags_None);
			ImGui::SliderFloat3("Size", glm::value_ptr(transform.Size), -10.0f,
				10.0f, "%.3f", ImGuiSliderFlags_None);
		}
	}

	if (selectedEntity.hasComponent<ColourComponent>()) {
		if (ImGui::CollapsingHeader("Colour", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::ColorEdit3("Colour",
				glm::value_ptr(
					selectedEntity.getComponent<ColourComponent>().Colour));
		}
	}

	if (selectedEntity.hasComponent<OrthoCameraComponent>()) {
		OrthoCameraComponent& orthoCamera =
			selectedEntity.getComponent<OrthoCameraComponent>();

		if (ImGui::CollapsingHeader(
				"Ortho Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::SliderFloat4("Bounds", glm::value_ptr(orthoCamera.Bounds),
				-100.0f, 100.0f, "%.3f", ImGuiSliderFlags_None);
			ImGui::SliderFloat2("Planes", glm::value_ptr(orthoCamera.Planes),
				-100.0f, 100.0f, "%.3f", ImGuiSliderFlags_None);
		}
	}

	ImGui::End();
}

static void GameViewport(Game& game, Renderer& renderer, ImVec2& viewportSize) {
	ImGui::Begin("Game", NULL, ImGuiWindowFlags_NoScrollbar);
	ImVec2 screenSize = ImGui::GetContentRegionAvail();
	Framebuffer* framebuffer = game.getFramebuffer();
	Framebuffer* framebufferMultisampled = game.getFramebufferMultisampled();

	ImGui::Image(
		ImTextureID((void*)(uintptr_t)framebuffer->getColourTextureId()),
		ImVec2(screenSize.x, screenSize.y), ImVec2(0, 1), ImVec2(1, 0));

	if (screenSize.x != viewportSize.x || screenSize.y != viewportSize.y) {
		viewportSize = screenSize;
		renderer.setViewport({0.0f, 0.0f}, {screenSize.x, screenSize.y});
		game.getCamera().setAspectRatio(screenSize.x / screenSize.y);
		framebuffer->resize(screenSize.x, screenSize.y);
		framebufferMultisampled->resize(screenSize.x, screenSize.y);
	}

	ImGui::End();
}

} // namespace ImGuiCustom

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

void ImGuiLayer::onUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	Game& game = Game::getInstance();
	std::vector<Entity> entities = game.getScene()->getAllEntities();
	ImGuiCustom::EntityList(entities, m_selectedEntity);
	ImGuiCustom::ComponentEditor(m_selectedEntity);

	Renderer& renderer = Renderer::getInstance();

	ImGuiCustom::GameViewport(game, renderer, m_viewportSize);

	ImGui::Begin("Depth Map", NULL, ImGuiWindowFlags_NoScrollbar);
	ImVec2 screenSize = ImGui::GetContentRegionAvail();
	ImGui::Image(
		ImTextureID((void*)(uintptr_t)game.getDepthMap()->getColourTextureId()),
		ImVec2(screenSize.x, screenSize.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::PopStyleVar();
	ImGui::Begin("Performance");
	ImGui::Text("FPS: %.2f", game.getFPS());
	ImGui::End();

	ImGui::Render();

	if (ImGui::IsKeyPressed(GLFW_KEY_L, false)) {
		m_gameMode = !m_gameMode;
		Window::getInstance().setMouseEnabled(!m_gameMode);
		((PerspectiveCamera&)game.getCamera()).enableMouse(m_gameMode);
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace MattEngine