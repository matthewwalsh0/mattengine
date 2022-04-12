#ifndef LAYER_H
#define LAYER_H

namespace MattEngine {

class Layer {
public:
	virtual void onInit() = 0;
	virtual void onUpdate(){};
	virtual void onBeforeRender(){};
	virtual void onRender(){};
	virtual void onAfterRender(){};
	virtual void onResize(unsigned int width, unsigned int height){};
	virtual void onInactive(float deltaTime){};
	virtual ~Layer(){};
};

} // namespace MattEngine

#endif
