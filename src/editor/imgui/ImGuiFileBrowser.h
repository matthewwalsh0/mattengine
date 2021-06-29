#include "imgui.h"

#include <filesystem>

namespace MattEngine {
namespace ImGuiCustom {

class FileBrowser {
public:
	void render();

private:
	std::filesystem::path m_rootPath = ".";
	std::filesystem::path m_currentPath = m_rootPath;
	int m_columnCount = 5;
};

} // namespace ImGuiCustom
} // namespace MattEngine