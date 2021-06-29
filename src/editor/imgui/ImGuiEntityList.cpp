#include "ImGuiEntityList.h"

#include "Game.h"

using namespace MattEngine::ImGuiCustom;

bool EntityList::render() {
	bool newEntitySelected = false;

	ImGui::Begin("Entities");
	ImGui::BeginListBox("##EntityList",
		ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing()));

	Game& game = Game::getInstance();
	std::vector<Entity> sceneEntities = game.getScene().getAllEntities();

	for (auto& entity : sceneEntities) {
		if (!entity.hasComponent<TagComponent>())
			continue;

		TagComponent& tag = entity.getComponent<TagComponent>();
		bool isSelected = m_selectedEntity == entity;

		if (ImGui::Selectable(tag.Tag.c_str(), isSelected)) {
			m_selectedEntity = entity;
			newEntitySelected = true;
		}
	}

	ImGui::EndListBox();
	ImGui::End();

	return newEntitySelected;
}