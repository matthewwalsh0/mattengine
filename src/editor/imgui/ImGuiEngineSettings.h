#ifndef ENGINE_SETTINGS_H
#define ENGINE_SETTINGS_H

#include "ImGuiPerspectiveCamera.h"

#include "imgui.h"

namespace MattEngine::ImGuiCustom {

class EngineSettings {
public:
	void render();

public:
	bool RenderCameraBounds = false;
	bool RenderShadowCameraBounds = false;

private:
	ImGuiCustom::ImGuiPerspectiveCamera m_perspectiveCamera;
};

} // namespace MattEngine::ImGuiCustom

#endif
