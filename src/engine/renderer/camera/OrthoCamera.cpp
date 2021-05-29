#include "OrthoCamera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace MattEngine;

OrthoCamera::OrthoCamera(const glm::vec4 bounds, const glm::vec2 planes,
	const glm::vec3 position, const glm::vec3 lookAt)
	: m_bounds(bounds), m_planes(planes), m_position(position),
	  m_lookAt(lookAt) {
	invalidate();
}

void OrthoCamera::invalidate() {
	m_projection = glm::ortho(m_bounds[0], m_bounds[1], m_bounds[2],
		m_bounds[3], m_planes[0], m_planes[1]);

	m_view = glm::lookAt(m_position, m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
}