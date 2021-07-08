#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Layer.h"

namespace MattEngine {

class ImGuiLayer : public Layer {
public:
	void onInit() override;
	void onAfterRender() override;

	virtual void onRender() = 0;
};
} // namespace MattEngine

#endif
