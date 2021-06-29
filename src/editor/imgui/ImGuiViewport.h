#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "imgui.h"

namespace MattEngine::ImGuiCustom {

class Viewport {
public:
	void render();
	void reset() { m_viewportSize = {0, 0}; }

private:
	ImVec2 m_viewportSize{0, 0};
};

} // namespace MattEngine::ImGuiCustom

#endif
