#include "ImGuiPerspectiveCamera.h"

#include <glm/gtc/type_ptr.hpp>

using namespace MattEngine::ImGuiCustom;

void ImGuiPerspectiveCamera::render(PerspectiveCamera& camera) {
	glm::vec3 position = camera.getPosition();

	ImGui::DragFloat3(
		"Position", glm::value_ptr(position), 0.1f, -1000.0f, 1000.0f);

	glm::quat rotation = camera.getRotation();
	m_quatEditor.render(rotation, true);
}