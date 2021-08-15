#ifndef POST_PROCESSING_LAYER_H
#define POST_PROCESSING_LAYER_H

#include "Framebuffer.h"
#include "Layer.h"
#include "Shader.h"

#include <memory>
#include <vector>

namespace MattEngine {

class PostProcessingLayer : public Layer {
public:
	void onInit() override;
	void onAfterRender() override;
	void onResize(unsigned int width, unsigned int height) override;

private:
	void brightnessFilter(
		Framebuffer& source, Framebuffer& target, float threshold);
	void toneMapping(Framebuffer& source, Framebuffer& target, float exposure);
	void blur(Framebuffer& source, Framebuffer& target, bool horizontal);
	void copy(Framebuffer& source, Framebuffer& target);
	void add(Framebuffer& source1, Framebuffer& source2, Framebuffer& target);
	void gray(Framebuffer& source1, Framebuffer& source2, Framebuffer& target);

public:
	bool Bloom = true;
	unsigned int BloomBlurPasses = 4;
	float BloomThreshold = 1.0f;
	float BloomBlurSize = 0.6f;
	bool ToneMapping = true;
	float Exposure = 1.0f;
	bool Grayscale = false;
	bool Invert = false;

private:
	Framebuffer* m_framebuffer;
	Framebuffer* m_framebuffer_2;
	std::vector<std::shared_ptr<Framebuffer>> m_bloomFramebuffers;

	Shader m_shaderGrayscale = Shader("assets/shaders/grayscale.glsl");
	Shader m_shaderInvert = Shader("assets/shaders/invert.glsl");
	Shader m_shaderBlur = Shader("assets/shaders/blur.glsl");
	Shader m_shaderBrightnessFilter =
		Shader("assets/shaders/brightnessFilter.glsl");
	Shader m_shaderBloom = Shader("assets/shaders/bloom.glsl");
	Shader m_shaderHdr = Shader("assets/shaders/hdr.glsl");
	Shader m_shaderTexture = Shader("assets/shaders/texture.glsl");
};

} // namespace MattEngine

#endif
