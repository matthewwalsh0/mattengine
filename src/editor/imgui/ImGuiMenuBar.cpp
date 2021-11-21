#include "ImGuiMenuBar.h"

#include "Dialogs.h"
#include "Game.h"
#include "SceneExporter.h"
#include "SceneLoader.h"
#include "Window.h"

using namespace MattEngine::ImGuiCustom;

void MenuBar::render() {
	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Open...", "CMD+O"))
				onOpenItemClick();

			ImGui::Separator();

			if (ImGui::MenuItem("Save", "CMD+S", false, !!m_currentPath))
				onSaveItemClick();

			if (ImGui::MenuItem("Save As...", "CMD+SHIFT+S"))
				onSaveAsItemClick();

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();

	if (ImGui::IsKeyPressed(GLFW_KEY_O) && ImGui::GetIO().KeySuper)
		onOpenItemClick();

	if (ImGui::IsKeyPressed(GLFW_KEY_S) && ImGui::GetIO().KeySuper)
		onSaveItemClick();

	if (ImGui::IsKeyPressed(GLFW_KEY_S) && ImGui::GetIO().KeySuper &&
		ImGui::GetIO().KeyShift)
		onSaveAsItemClick();
}

void MenuBar::onOpenItemClick() {
	auto openPath = Dialogs::open();

	if (!openPath)
		return;

	m_currentPath = openPath;

	Game& game = Game::getInstance();
	auto scene = SceneLoader::load(*openPath);

	game.loadScene(scene);
	updateTitle();
}

void MenuBar::onSaveItemClick() {
	if (!m_currentPath) {
		onSaveAsItemClick();
		return;
	}

	Game& game = Game::getInstance();
	SceneExporter::save(game.getScene(), *m_currentPath);
}

void MenuBar::onSaveAsItemClick() {
	auto savePath = Dialogs::save();

	if (!savePath)
		return;

	m_currentPath = savePath;

	Game& game = Game::getInstance();
	SceneExporter::save(game.getScene(), *m_currentPath);
	updateTitle();
}

void MenuBar::updateTitle() {
	if (!m_currentPath)
		return;

	MattEngine::Window::getInstance().setTitle(
		"MattEngine - " + *m_currentPath);
}