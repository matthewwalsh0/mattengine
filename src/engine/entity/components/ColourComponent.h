#ifndef COLOUR_COMPONENT_H
#define COLOUR_COMPONENT_H

#include <glm/glm.hpp>

struct ColourComponent {
	glm::vec3 Colour;

	ColourComponent(const glm::vec3& colour) : Colour(colour) {}
};

#endif