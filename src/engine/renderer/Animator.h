#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Model.h"

#include <glm/glm.hpp>

#include <vector>

namespace MattEngine {

class Animator {
public:
	Animator();
	void onUpdate(float deltaTime);
	void playAnimation(Animation& animation);
	std::vector<glm::mat4>& getBoneTransforms();

	float getCurrentTime() { return m_currentTime; }

private:
	std::vector<glm::mat4> m_boneTransforms;
	Animation* m_animation = nullptr;
	float m_currentTime;
};

} // namespace MattEngine

#endif
