#include "PostProcessingLayer.h"

#include "Game.h"
#include "Renderer.h"

namespace MattEngine {

static void clearFramebuffer(Framebuffer& target) {
	Renderer& renderer = Renderer::getInstance();

	target.bind();
	renderer.setViewport({0.0f, 0.0f}, {target.getWidth(), target.getHeight()});
	renderer.clear();
}

static void drawToFramebuffer(Framebuffer& target) {
	Renderer& renderer = Renderer::getInstance();

	target.bind();
	renderer.setViewport({0.0f, 0.0f}, {target.getWidth(), target.getHeight()});
	renderer.clear();

	renderer.drawFramebuffer();
}

static void basicShaderPass(
	Framebuffer& source, Framebuffer& target, Shader& shader) {
	unsigned int sourceSlot = 2;

	shader.bind();
	shader.setInt("u_Texture", sourceSlot);

	Texture(source.getColourTextureId()).bind(sourceSlot);

	drawToFramebuffer(target);
}

void PostProcessingLayer::brightnessFilter(
	Framebuffer& source, Framebuffer& target, float threshold) {
	int sourceTextureSlot = 2;

	m_shaderBrightnessFilter.bind();
	m_shaderBrightnessFilter.setInt("u_Texture", sourceTextureSlot);
	m_shaderBrightnessFilter.setFloat("u_Threshold", threshold);

	Texture(source.getColourTextureId()).bind(sourceTextureSlot);

	drawToFramebuffer(target);
}

void PostProcessingLayer::toneMapping(
	Framebuffer& source, Framebuffer& target, float exposure) {
	int sourceTextureSlot = 2;

	m_shaderHdr.bind();
	m_shaderHdr.setFloat("u_Exposure", exposure);
	m_shaderHdr.setInt("u_Texture", sourceTextureSlot);

	Texture(source.getColourTextureId()).bind(sourceTextureSlot);

	drawToFramebuffer(target);
}

void PostProcessingLayer::blur(
	Framebuffer& source, Framebuffer& target, bool horizontal) {
	int sourceTextureSlot = 2;

	m_shaderBlur.bind();
	m_shaderBlur.setBool("u_Horizontal", horizontal);
	m_shaderBlur.setInt("u_Texture", sourceTextureSlot);

	Texture(source.getColourTextureId()).bind(sourceTextureSlot);

	drawToFramebuffer(target);
}

void PostProcessingLayer::copy(Framebuffer& source, Framebuffer& target) {
	int sourceTextureSlot = 2;

	m_shaderTexture.bind();
	m_shaderTexture.setInt("u_Texture", sourceTextureSlot);

	Texture(source.getColourTextureId()).bind(sourceTextureSlot);

	drawToFramebuffer(target);
}

void PostProcessingLayer::add(
	Framebuffer& source1, Framebuffer& source2, Framebuffer& target) {
	int sourceSlot_1 = 2;
	int sourceSlot_2 = 3;

	m_shaderBloom.bind();
	m_shaderBloom.setInt("u_Texture_1", sourceSlot_1);
	m_shaderBloom.setInt("u_Texture_2", sourceSlot_2);

	Texture(source1.getColourTextureId()).bind(sourceSlot_1);
	Texture(source2.getColourTextureId()).bind(sourceSlot_2);

	drawToFramebuffer(target);
}

void PostProcessingLayer::onInit() {
	m_framebuffer = new Framebuffer(640, 480);
	m_framebuffer_2 = new Framebuffer(640, 480);
}

void PostProcessingLayer::onResize(unsigned int width, unsigned int height) {
	m_framebuffer->resize(width, height);
	m_framebuffer_2->resize(width, height);
	m_bloomFramebuffers.clear();

	for (int i = 0; i < BloomBlurPasses; i++) {
		m_bloomFramebuffers.emplace_back(
			new Framebuffer((unsigned int)(width * pow(BloomBlurSize, i + 1)),
				(unsigned int)(height * pow(BloomBlurSize, i + 1))));

		m_bloomFramebuffers.emplace_back(
			new Framebuffer((unsigned int)(width * pow(BloomBlurSize, i + 1)),
				(unsigned int)(height * pow(BloomBlurSize, i + 1))));
	}
}

void PostProcessingLayer::onAfterRender() {
	Game& game = Game::getInstance();
	Renderer& renderer = Renderer::getInstance();
	Framebuffer* inputFramebuffer = game.getFramebuffer();

	if (Bloom) {
		brightnessFilter(*inputFramebuffer, *m_framebuffer, BloomThreshold);

		Framebuffer* currentInput = m_framebuffer;

		for (int i = 0; i < BloomBlurPasses; i++) {
			blur(*currentInput, *m_bloomFramebuffers[i * 2], true);
			blur(*m_bloomFramebuffers[i * 2], *m_bloomFramebuffers[i * 2 + 1],
				false);

			currentInput = &*m_bloomFramebuffers[i * 2 + 1];
		}

		m_bloomFramebuffers[1]->copy(*m_framebuffer);

		for (int i = 0; i < BloomBlurPasses - 1; i++) {
			add(*m_framebuffer, *m_bloomFramebuffers[(i + 1) * 2 + 1],
				*m_framebuffer_2);

			m_framebuffer_2->copy(*m_framebuffer);
		}

		add(*inputFramebuffer, *m_framebuffer, *m_framebuffer_2);
		m_framebuffer_2->copy(*inputFramebuffer);
	}

	if (Grayscale) {
		basicShaderPass(*inputFramebuffer, *m_framebuffer, m_shaderGrayscale);
		m_framebuffer->copy(*inputFramebuffer);
	}

	if (Invert) {
		basicShaderPass(*inputFramebuffer, *m_framebuffer, m_shaderInvert);
		m_framebuffer->copy(*inputFramebuffer);
	}

	if (ToneMapping) {
		toneMapping(*inputFramebuffer, *m_framebuffer, Exposure);
		m_framebuffer->copy(*inputFramebuffer);
	}

	inputFramebuffer->copyToScreen();
	m_framebuffer->unbind();
}

} // namespace MattEngine