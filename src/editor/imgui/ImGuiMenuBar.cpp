#include "ImGuiMenuBar.h"

#include "Dialogs.h"
#include "Game.h"
#include "SceneExporter.h"

using namespace MattEngine::ImGuiCustom;

void MenuBar::render() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open...")) {
				if (auto openPath = Dialogs::open()) {
					SceneExporter::load(*openPath);
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save As")) {
				if (auto savePath = Dialogs::save()) {
					Game& game = Game::getInstance();
					SceneExporter::save(game.getScene(), *savePath);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}