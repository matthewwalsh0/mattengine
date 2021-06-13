#ifndef LAYER_H
#define LAYER_H

namespace MattEngine {

class Layer {
public:
	virtual void onInit() = 0;
	virtual void onUpdate(){};
	virtual void onBeforeRender(){};
	virtual void onAfterRender(){};
	virtual ~Layer(){};
};

} // namespace MattEngine

#endif
