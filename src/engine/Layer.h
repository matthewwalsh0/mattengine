#ifndef LAYER_H
#define LAYER_H

namespace MattEngine {

class Layer {
public:
	virtual void onInit(){};
	virtual void onUpdate(){};
};

} // namespace MattEngine

#endif
