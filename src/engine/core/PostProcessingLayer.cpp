#include "PostProcessingLayer.h"

#include "Game.h"
#include "Renderer.h"

namespace MattEngine {

static void drawFramebuffer(Framebuffer& source, Framebuffer& target) {
	Renderer& renderer = Renderer::getInstance();

	target.bind();
	renderer.setViewport({0.0f, 0.0f}, {target.getWidth(), target.getHeight()});
	renderer.clear();

	renderer.drawFramebuffer(source);
}

void PostProcessingLayer::onInit() {
	m_framebuffer = new Framebuffer(640, 480);
	m_framebuffer_2 = new Framebuffer(640, 480);
	m_framebuffer_3 = new Framebuffer(640, 480);
}

void PostProcessingLayer::onResize(unsigned int width, unsigned int height) {
	m_framebuffer->resize(width * BloomBlurSize, height * BloomBlurSize);
	m_framebuffer_2->resize(width * BloomBlurSize, height * BloomBlurSize);
	m_framebuffer_3->resize(width, height);
}

void PostProcessingLayer::onAfterRender() {
	Game& game = Game::getInstance();
	Renderer& renderer = Renderer::getInstance();

	Framebuffer* primaryFramebuffer = game.getFramebuffer();

	if (Grayscale) {
		m_shaderGrayscale.bind();
		m_shaderGrayscale.setInt("u_Texture", 1);

		drawFramebuffer(*primaryFramebuffer, *m_framebuffer_3);
		m_framebuffer_3->copy(*primaryFramebuffer);
	}

	if (Invert) {
		m_shaderInvert.bind();
		m_shaderInvert.setInt("u_Texture", 1);

		drawFramebuffer(*primaryFramebuffer, *m_framebuffer_3);
		m_framebuffer_3->copy(*primaryFramebuffer);
	}

	if (Bloom) {
		Texture(primaryFramebuffer->getColourTextureId()).bind(2);

		m_shaderBrightnessFilter.bind();
		m_shaderBrightnessFilter.setInt("u_Texture", 1);
		m_shaderBrightnessFilter.setFloat("u_Threshold", BloomThreshold);
		drawFramebuffer(*primaryFramebuffer, *m_framebuffer);

		m_shaderBlur.bind();
		m_shaderBlur.setInt("u_Texture", 1);

		for (int i = 0; i < BloomBlurPasses; i++) {
			m_shaderBlur.setBool("u_Horizontal", true);
			drawFramebuffer(*m_framebuffer, *m_framebuffer_2);

			m_shaderBlur.setBool("u_Horizontal", false);
			drawFramebuffer(*m_framebuffer_2, *m_framebuffer);
		}

		m_shaderBloom.bind();
		m_shaderBloom.setInt("u_Texture", 2);
		m_shaderBloom.setInt("u_TextureBloom", 1);
		drawFramebuffer(*m_framebuffer, *m_framebuffer_3);

		m_framebuffer_3->copy(*primaryFramebuffer);
	}

	primaryFramebuffer->copyToScreen();
	m_framebuffer->unbind();
}

} // namespace MattEngine