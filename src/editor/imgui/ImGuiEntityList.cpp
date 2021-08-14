#include "ImGuiEntityList.h"

#include "Game.h"

using namespace MattEngine::ImGuiCustom;

bool EntityList::render() {
	bool newEntitySelected = false;
	Game& game = Game::getInstance();

	ImGui::Begin("Entities");

	if (ImGui::Button("Add Entity")) {
		Entity entity = game.getScene().createEntity();
		int entityCount = game.getScene().getAllEntities().size();
		entity.addComponent<TagComponent>(
			"Entity " + std::to_string(entityCount));
	}

	ImGui::BeginListBox("##EntityList",
		ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing()));

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