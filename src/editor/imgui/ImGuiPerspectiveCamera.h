#ifndef IMGUI_PERSPECTIVE_CAMERA_H
#define IMGUI_PERSPECTIVE_CAMERA_H

#include "ImGuiQuatEditor.h"
#include "PerspectiveCamera.h"

#include "imgui.h"

namespace MattEngine::ImGuiCustom {

class ImGuiPerspectiveCamera {
public:
	void render(PerspectiveCamera& camera);

private:
	QuatEditor m_quatEditor;
};

} // namespace MattEngine::ImGuiCustom

#endif
