#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "ImGuiLayer.h"

#include "ImGuiComponentEditor.h"
#include "ImGuiEngineSettings.h"
#include "ImGuiEntityList.h"
#include "ImGuiFileBrowser.h"
#include "ImGuiMenuBar.h"
#include "ImGuiPerspectiveCamera.h"
#include "ImGuiViewport.h"
#include "PerspectiveCamera.h"

namespace MattEngine {

enum State { EDITOR, GAME, UPDATE };

class EditorLayer : public ImGuiLayer {
public:
	void onInit() override;
	void onRender() override;
	void onResize(unsigned int width, unsigned int height) override;
	void onInactive(float deltaTime) override;

private:
	ImGuiCustom::FileBrowser m_fileBrowser;
	ImGuiCustom::EntityList m_entityList;
	ImGuiCustom::ComponentEditor m_componentEditor;
	ImGuiCustom::Viewport m_gameViewport;
	ImGuiCustom::MenuBar m_menuBar;
	ImGuiCustom::EngineSettings m_engineSettings;
	PerspectiveCamera m_editorCamera;
	PerspectiveCamera* m_gameCamera;
	State m_state = State::EDITOR;
};
} // namespace MattEngine

#endif
