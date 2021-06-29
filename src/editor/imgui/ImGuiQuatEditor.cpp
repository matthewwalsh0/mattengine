#include "ImGuiQuatEditor.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

using namespace MattEngine::ImGuiCustom;

void QuatEditor::render(glm::quat& rotation, bool newQuat) {
	if (newQuat) {
		glm::vec3 rotationEulerRadians = glm::eulerAngles(rotation);

		m_rotationEulerDegrees = glm::vec3(glm::degrees(rotationEulerRadians.x),
			glm::degrees(rotationEulerRadians.y),
			glm::degrees(rotationEulerRadians.z));
	}

	bool rotationChanged = ImGui::DragFloat3("Rotation",
		glm::value_ptr(m_rotationEulerDegrees), 5.0f, -180.0f, 180.0f);

	if (rotationChanged) {
		glm::quat updatedQuat =
			glm::quat(glm::vec3(glm::radians(m_rotationEulerDegrees.x),
				glm::radians(m_rotationEulerDegrees.y),
				glm::radians(m_rotationEulerDegrees.z)));

		rotation = updatedQuat;
	}
}