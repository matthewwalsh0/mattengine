#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace MattEngine {

class Light {
public:
	Light(const glm::vec3& position, const glm::vec3& colour)
		: m_position(position), m_colour(colour) {}
	glm::vec3& getPosition() { return m_position; }
	glm::vec3& getColour() { return m_colour; }

private:
	glm::vec3 m_position;
	glm::vec3 m_colour;
};

} // namespace MattEngine

#endif
