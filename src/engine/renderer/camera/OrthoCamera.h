#ifndef ORTHO_CAMERA_H
#define ORTHO_CAMERA_H

#include "Camera.h"

namespace MattEngine {

class OrthoCamera : public Camera {
public:
	OrthoCamera(const glm::vec4 bounds, const glm::vec2 planes,
		const glm::vec3 position, const glm::vec3 lookAt);

	const glm::mat4& getProjection() override { return m_projection; };
	const glm::mat4& getView() override { return m_view; };
	const glm::vec3& getPosition() override { return m_position; }

	void setPosition(const glm::vec3& position) {
		m_position = position;
		invalidate();
	}

private:
	void invalidate();

private:
	glm::vec4 m_bounds;
	glm::vec2 m_planes;
	glm::vec3 m_position;
	glm::vec3 m_lookAt;
	glm::mat4 m_projection;
	glm::mat4 m_view;
};

} // namespace MattEngine

#endif
