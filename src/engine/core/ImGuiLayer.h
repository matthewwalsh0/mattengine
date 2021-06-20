#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Entity.h"
#include "ImGuiFileBrowser.h"
#include "Layer.h"

#include "imgui.h"

namespace MattEngine {

class ImGuiLayer : public Layer {
public:
	void onInit() override;
	void onAfterRender() override;

private:
	bool m_gameMode = false;
	ImVec2 m_viewportSize{0, 0};
	Entity m_selectedEntity;
	ImGuiCustom::FileBrowser m_fileBrowser;
};
} // namespace MattEngine

#endif
