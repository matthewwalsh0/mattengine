#ifndef ENTITY_LIST_H
#define ENTITY_LIST_H

#include "Entity.h"

#include "imgui.h"

namespace MattEngine::ImGuiCustom {

class EntityList {
public:
	bool render();

public:
	std::optional<Entity> SelectedEntity;
};

} // namespace MattEngine::ImGuiCustom

#endif
