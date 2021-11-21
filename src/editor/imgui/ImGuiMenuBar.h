#ifndef MENU_BAR_H
#define MENU_BAR_H

#include "imgui.h"

#include <optional>
#include <string>

namespace MattEngine::ImGuiCustom {

class MenuBar {
public:
	void render();

private:
	void onOpenItemClick();
	void onSaveItemClick();
	void onSaveAsItemClick();
	void updateTitle();

private:
	std::optional<std::string> m_currentPath;
};

} // namespace MattEngine::ImGuiCustom

#endif
