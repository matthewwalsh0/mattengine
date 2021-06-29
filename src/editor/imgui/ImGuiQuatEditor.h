#ifndef QUAT_EDITOR_H
#define QUAT_EDITOR_H

#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace MattEngine::ImGuiCustom {

class QuatEditor {
public:
	void render(glm::quat& quat, bool newQuat);

private:
	glm::vec3 m_rotationEulerDegrees;
};

} // namespace MattEngine::ImGuiCustom

#endif
