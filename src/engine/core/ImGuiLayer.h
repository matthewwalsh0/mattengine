#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Entity.h"
#include "ImGuiComponentEditor.h"
#include "ImGuiEngineSettings.h"
#include "ImGuiEntityList.h"
#include "ImGuiFileBrowser.h"
#include "ImGuiMenuBar.h"
#include "ImGuiPerspectiveCamera.h"
#include "ImGuiViewport.h"
#include "Layer.h"

#include "imgui.h"

namespace MattEngine {

class ImGuiLayer : public Layer {
public:
	void onInit() override;
	void onAfterRender() override;

private:
	ImGuiCustom::FileBrowser m_fileBrowser;
	ImGuiCustom::EntityList m_entityList;
	ImGuiCustom::ComponentEditor m_componentEditor;
	ImGuiCustom::Viewport m_gameViewport;
	ImGuiCustom::MenuBar m_menuBar;
	ImGuiCustom::EngineSettings m_engineSettings;
};
} // namespace MattEngine

#endif
