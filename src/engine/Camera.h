#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace MattEngine {

class Camera {
public:
	virtual const glm::mat4& getProjection() = 0;
	virtual const glm::mat4& getView() = 0;
	virtual void onUpdate(float deltaTime){};
};

} // namespace MattEngine

#endif
