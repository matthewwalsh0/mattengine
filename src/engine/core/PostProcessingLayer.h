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
	bool Bloom = true;
	unsigned int BloomBlurPasses = 5;
	float BloomThreshold = 0.99f;
	float BloomBlurSize = 0.5f;

private:
	Framebuffer* m_framebuffer;
	Framebuffer* m_framebuffer_2;
	Framebuffer* m_framebuffer_3;
	Shader m_shaderGrayscale = Shader("assets/shaders/grayscale.glsl");
	Shader m_shaderInvert = Shader("assets/shaders/invert.glsl");
	Shader m_shaderBlur = Shader("assets/shaders/blur.glsl");
	Shader m_shaderBrightnessFilter =
		Shader("assets/shaders/brightnessFilter.glsl");
	Shader m_shaderBloom = Shader("assets/shaders/bloom.glsl");
};

} // namespace MattEngine

#endif
