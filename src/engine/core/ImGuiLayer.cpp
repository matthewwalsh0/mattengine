#include "ImGuiLayer.h"

#include "AnimationComponent.h"
#include "ColourComponent.h"
#include "Game.h"
#include "TransformComponent.h"
#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MattEngine {

namespace ImGuiCustom {

static bool EntityList(std::vector<Entity>& entities, Entity& selectedEntity) {
	bool newSelection = false;

	ImGui::Begin("Entities");
	ImGui::BeginListBox("##EntityList",
		ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()));

	Game& game = Game::getInstance();

	for (auto& entity : game.getScene()->getAllEntities()) {
		if (ImGui::Selectable(entity.getComponent<TagComponent>().Tag.c_str(),
				selectedEntity == entity)) {

			selectedEntity = entity;
			newSelection = true;
		}
	}

	ImGui::EndListBox();
	ImGui::End();

	return newSelection;
}

static bool QuatRotation(
	glm::quat& rotation, glm::vec3& rotationEulerDegrees, bool newSelection) {

	if (newSelection) {
		glm::vec3 rotationEulerRadians = glm::eulerAngles(rotation);

		rotationEulerDegrees = glm::vec3(glm::degrees(rotationEulerRadians.x),
			glm::degrees(rotationEulerRadians.y),
			glm::degrees(rotationEulerRadians.z));
	}

	bool rotationChanged = ImGui::DragFloat3("Rotation",
		glm::value_ptr(rotationEulerDegrees), 5.0f, -180.0f, 180.0f);

	if (rotationChanged) {
		glm::quat updatedQuat =
			glm::quat(glm::vec3(glm::radians(rotationEulerDegrees.x),
				glm::radians(rotationEulerDegrees.y),
				glm::radians(rotationEulerDegrees.z)));

		rotation = updatedQuat;

		return true;
	}

	return false;
}

static void PerspectiveCamera(PerspectiveCamera& camera) {
	glm::vec3 position = camera.getPosition();

	if (ImGui::DragFloat3(
			"Position", glm::value_ptr(position), 0.1f, -1000.0f, 1000.0f)) {
		camera.setPosition(position);
	}

	glm::quat rotation = camera.getRotation();
	static glm::vec3 rotationEulerDegrees;
	ImGuiCustom::QuatRotation(rotation, rotationEulerDegrees, true);
}

static void ComponentEditor(Entity& selectedEntity, bool newSelection) {
	if (!selectedEntity)
		return;

	ImGui::Begin("Components");

	if (selectedEntity.hasComponent<TransformComponent>()) {
		TransformComponent& transform =
			selectedEntity.getComponent<TransformComponent>();

		if (ImGui::CollapsingHeader(
				"Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::DragFloat3("Position", glm::value_ptr(transform.Position),
				0.1f, -1000.0f, 1000.0f);
			ImGui::DragFloat3("Size", glm::value_ptr(transform.Size), 0.1f,
				-1000.0f, 1000.0f);

			static glm::vec3 rotationEulerDegrees;
			QuatRotation(
				transform.Rotation, rotationEulerDegrees, newSelection);
		}
	}

	if (selectedEntity.hasComponent<ColourComponent>()) {
		if (ImGui::CollapsingHeader("Colour", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::ColorEdit3("Colour",
				glm::value_ptr(
					selectedEntity.getComponent<ColourComponent>().Colour));
		}
	}

	if (selectedEntity.hasComponent<AnimationComponent>()) {
		if (ImGui::CollapsingHeader(
				"Animation", ImGuiTreeNodeFlags_DefaultOpen)) {

			AnimationComponent& animation =
				selectedEntity.getComponent<AnimationComponent>();

			float currentTime = animation.Animator.getCurrentTime();
			float duration = animation.Animation->Duration;
			float ticksPerSecond = animation.Animation->TicksPerSecond;
			auto bones = animation.Animation->BonesByName;
			float boneCount = bones.size();

			ImGui::DragFloat("Duration", &duration);
			ImGui::DragFloat("Ticks Per Second", &ticksPerSecond);
			ImGui::DragFloat("Current Time", &currentTime);
			ImGui::DragFloat("Bone Count", &boneCount);
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
		game.resize({screenSize.x, screenSize.y});
	}

	if (!game.isActive()) {
		Window::getInstance().setFocused(ImGui::IsWindowHovered());
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

		m_viewportSize = {0.0f, 0.0f};
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

	std::vector<Entity> entities = game.getScene()->getAllEntities();
	bool newSelection = ImGuiCustom::EntityList(entities, m_selectedEntity);
	ImGuiCustom::ComponentEditor(m_selectedEntity, newSelection);

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

	ImGui::Begin("Engine", NULL, ImGuiWindowFlags_NoScrollbar);

	Framebuffer* framebufferMultisampled = game.getFramebufferMultisampled();

	int samples = game.getFramebufferMultisampled()->getSamples();
	bool samplesChanged = ImGui::SliderInt("MSAA", &samples, 0, 16);

	if (samplesChanged) {
		framebufferMultisampled->setSamples(samples);
	}

	ImGui::End();

	ImGui::Begin("Camera", NULL, ImGuiWindowFlags_NoScrollbar);
	ImGui::Text("Game");
	ImGui::Spacing();
	ImGuiCustom::PerspectiveCamera(game.getCamera());
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("Editor");
	ImGui::Spacing();
	ImGuiCustom::PerspectiveCamera(game.getEditorCamera());
	ImGui::End();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace MattEngine