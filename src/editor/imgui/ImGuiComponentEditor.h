#ifndef COMPONENT_EDITOR_H
#define COMPONENT_EDITOR_H

#include "Entity.h"
#include "ImGuiQuatEditor.h"

#include "imgui.h"

namespace MattEngine::ImGuiCustom {

class ComponentEditor {
public:
	void render(Entity& entity, bool newEntity);

private:
	QuatEditor m_quatEditor;
};

} // namespace MattEngine::ImGuiCustom

#endif
