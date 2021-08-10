#include "ImGuiFileBrowser.h"

#include "TextureStore.h"

#include <string>
#include <vector>

using namespace MattEngine::ImGuiCustom;

static std::string getPathName(
	const std::filesystem::path& path, const std::filesystem::path& rootPath) {
	return path == rootPath ? "Root" : path.filename().string();
}

static void ListDirectories(std::filesystem::path path,
	std::filesystem::path& currentPath, std::filesystem::path& rootPath) {
	int flags =
		ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

	if (path == currentPath) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool nodeExpanded =
		ImGui::TreeNodeEx(getPathName(path, rootPath).c_str(), flags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
		currentPath = path;
	}

	if (nodeExpanded) {
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			std::filesystem::path nestedPath = entry.path();

			if (std::filesystem::is_directory(nestedPath)) {
				ListDirectories(nestedPath, currentPath, rootPath);
			}
		}

		ImGui::TreePop();
	}
}

void FileBrowser::render() {
	std::vector<std::filesystem::path> parts;
	parts.push_back(m_currentPath);

	std::filesystem::path current = m_currentPath;

	while (current.has_parent_path()) {
		current = current.parent_path();
		parts.insert(parts.begin(), current);
	}

	ImGui::Begin("Files", NULL,
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	for (auto& part : parts) {
		if (ImGui::Button(getPathName(part, m_rootPath).c_str())) {
			m_currentPath = part;
		}

		if (part != m_currentPath) {
			ImGui::SameLine();
			ImGui::TextUnformatted(">");
			ImGui::SameLine();
		}
	}

	if (ImGui::BeginTable("FileBrowserLayout", 2,
			ImGuiTableFlags_Resizable & ~ImGuiTableFlags_ScrollY)) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::BeginChild("ScrollingDirectories");
		ListDirectories(std::filesystem::path("."), m_currentPath, m_rootPath);
		ImGui::EndChild();

		ImGui::TableSetColumnIndex(1);

		int x = 0;

		float tableWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = tableWidth / m_thumbnailWidth;

		ImVec2 cellPadding(10.0f, 10.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cellPadding);

		if (ImGui::BeginTable("Files", columnCount,
				ImGuiTableFlags_ScrollY | ImGuiTableFlags_PadOuterX)) {
			ImGui::TableNextRow();

			for (const auto& entry :
				std::filesystem::directory_iterator(m_currentPath)) {
				std::filesystem::path nestedPath = entry.path();

				if (!std::filesystem::is_directory(nestedPath)) {
					ImGui::TableSetColumnIndex(x);

					Texture fileIcon = TextureStore::getInstance().getTexture(
						"assets/textures/file.png");
					std::string extension = nestedPath.extension().string();
					float aspectRatio = 1.0f;
					float available = ImGui::GetContentRegionAvail().x;
					float thumbnailWidth = available;
					float thumbnailHeight = available;
					ImVec2 cursor = ImGui::GetCursorPos();
					float startX = cursor.x;
					float startY = cursor.y;

					if (extension == ".jpg" || extension == ".png") {
						std::string pathString = nestedPath.string();
						fileIcon =
							TextureStore::getInstance().getTexture(pathString);
						aspectRatio =
							(float)fileIcon.getHeight() / fileIcon.getWidth();

						if (aspectRatio > 1.0f) {
							thumbnailHeight = available;
							thumbnailWidth = thumbnailHeight / aspectRatio;
							startX += (available - thumbnailWidth) / 2;
						} else {
							thumbnailWidth = available;
							thumbnailHeight = thumbnailWidth * aspectRatio;
							startY += (available - thumbnailHeight) / 2;
						}

						ImGui::SetCursorPos({startX, startY});
					}

					ImGui::Image((void*)(uintptr_t)fileIcon.getId(),
						{thumbnailWidth, thumbnailHeight});

					if (ImGui::BeginDragDropSource(
							ImGuiDragDropFlags_None |
							ImGuiDragDropFlags_SourceAllowNullID)) {
						ImGui::SetDragDropPayload("FILE_PATH",
							(void*)nestedPath.c_str(),
							nestedPath.string().length() + 1);

						ImGui::Image((void*)(uintptr_t)fileIcon.getId(),
							{thumbnailWidth, thumbnailHeight});
						ImGui::EndDragDropSource();
					}

					ImGui::SetCursorPosY(cursor.y + available + 10.0f);
					ImGui::TextWrapped("%s", nestedPath.filename().c_str());

					x++;

					if (x == columnCount) {
						ImGui::TableNextRow();
						x = 0;
					}
				}
			}
		}

		ImGui::EndTable();
		ImGui::PopStyleVar();
	}

	ImGui::EndTable();
	ImGui::End();
}