#ifndef POINT_LIGHT_COMPONENT_H
#define POINT_LIGHT_COMPONENT_H

#include "PointLight.h"

namespace MattEngine {

struct PointLightComponent {
	PointLight Light;

	PointLightComponent() {}
	PointLightComponent(PointLight light) : Light(light) {}
};

} // namespace MattEngine

#endif