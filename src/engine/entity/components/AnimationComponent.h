#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "Animator.h"

namespace MattEngine {

struct AnimationComponent {
	bool Started;
	Animator Animator;
	Animation* Animation;

	AnimationComponent(MattEngine::Animation* animation)
		: Animation(animation), Started(false) {}
};

} // namespace MattEngine

#endif