#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include <vector>

namespace MattEngine {

class Camera {
public:
	virtual const glm::mat4& getProjection() = 0;
	virtual const glm::mat4& getView() = 0;
	virtual const glm::vec3& getPosition() = 0;
	virtual void onUpdate(float deltaTime){};
	virtual const void setAspectRatio(float aspectRatio){};

	glm::mat4 getProjectionView() { return getProjection() * getView(); }

	const std::vector<glm::vec3>& getBounds() {
		m_bounds.clear();

		glm::mat4 projectionInverse = glm::inverse(getProjectionView());

		std::vector<glm::vec4> cube = {{-1.0f, -1.0f, -1.0f, 1.0f},
			{-1.0f, 1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, -1.0f, 1.0f},
			{1.0f, -1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, -1.0f, 1.0f, 1.0f}};

		for (int vertexIndex = 0; vertexIndex < cube.size(); vertexIndex++) {
			glm::vec4 rawBound = projectionInverse * cube.at(vertexIndex);
			rawBound /= rawBound.w;

			glm::vec3 bound = glm::vec3(rawBound.x, rawBound.y, rawBound.z);
			m_bounds.push_back(bound);
		}

		return m_bounds;
	}

private:
	std::vector<glm::vec3> m_bounds;
};

} // namespace MattEngine

#endif
