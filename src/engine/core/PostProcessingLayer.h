#ifndef POST_PROCESSING_LAYER_H
#define POST_PROCESSING_LAYER_H

#include "Framebuffer.h"
#include "Layer.h"
#include "Shader.h"

#include <vector>

namespace MattEngine {

class PostProcessingLayer : public Layer {
public:
	void onInit() override;
	void onAfterRender() override;
	void onResize(unsigned int width, unsigned int height) override;

public:
	bool Grayscale = false;
	bool Invert = false;

private:
	Framebuffer* m_framebuffer;
	Shader m_shaderGrayscale = Shader("assets/shaders/grayscale.glsl");
	Shader m_shaderInvert = Shader("assets/shaders/invert.glsl");
};

} // namespace MattEngine

#endif
