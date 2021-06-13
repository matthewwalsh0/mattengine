#ifndef SHADOW_LAYER_H
#define SHADOW_LAYER_H

#include "Framebuffer.h"
#include "Layer.h"

#include <vector>

namespace MattEngine {

class ShadowLayer : public Layer {
public:
	void onInit() override;
	void onBeforeRender() override;

	std::vector<Framebuffer>& getDepthMaps() { return m_depthMaps; }

private:
	unsigned int m_depthMapCount = 4;
	std::vector<Framebuffer> m_depthMaps;
	std::vector<unsigned int> m_depthMapSizes = {2048, 1024, 1024, 1024};
	std::vector<float> m_depthMapFarPlanes = {10.0f, 40.0f, 100.0f, 1000.0f};
};

} // namespace MattEngine

#endif
