#ifndef ENTITY_LIST_H
#define ENTITY_LIST_H

#include "Entity.h"

#include "imgui.h"

namespace MattEngine::ImGuiCustom {

class EntityList {
public:
	bool render();

	Entity getSelectedEntity() { return m_selectedEntity; }

private:
	Entity m_selectedEntity;
};

} // namespace MattEngine::ImGuiCustom

#endif
