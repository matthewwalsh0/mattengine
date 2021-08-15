#ifndef COLOUR_COMPONENT_H
#define COLOUR_COMPONENT_H

#include <glm/glm.hpp>

struct ColourComponent {
	glm::vec3 Colour = {1.0f, 1.0f, 1.0f};
	bool Emissive = false;
	float Intensity = 1.0f;

	ColourComponent() {}
	ColourComponent(const glm::vec3& colour) : Colour(colour) {}
	ColourComponent(const glm::vec3& colour, float intensity)
		: Colour(colour), Emissive(true), Intensity(intensity) {}
};

#endif