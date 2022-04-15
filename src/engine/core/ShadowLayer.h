#ifndef SHADOW_LAYER_H
#define SHADOW_LAYER_H

#include "Framebuffer.h"
#include "Layer.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"

#include <vector>

namespace MattEngine {

class ShadowLayer : public Layer {
public:
	void onInit() override;
	void onBeforeRender() override;

	OrthoCamera getShadowCamera(PerspectiveCamera& camera,
		const glm::vec3& lightPosition, int depthMapIndex);
	const glm::vec3 getCascadeColour(int depthMapIndex);
	const glm::vec3 getLightPosition();

public:
	bool Enabled = true;
	bool CascadeIndicatorsEnabled = false;
	unsigned int DepthMapCount = 4;
	std::vector<Framebuffer> DepthMaps;
	std::vector<unsigned int> DepthMapSizes = {2048, 1024, 1024, 1024};
	std::vector<float> DepthMapFarPlanes = {10.0f, 40.0f, 100.0f, 1000.0f};

public:
	inline static ShadowLayer& getInstance() { return *s_instance; }

private:
	inline static ShadowLayer* s_instance;
};

} // namespace MattEngine

#endif
