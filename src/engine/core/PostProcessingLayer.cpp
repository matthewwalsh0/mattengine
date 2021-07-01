#include "PostProcessingLayer.h"

#include "Game.h"
#include "Renderer.h"

namespace MattEngine {

void PostProcessingLayer::onInit() {
	m_framebuffer = new Framebuffer(640, 480);
}

void PostProcessingLayer::onResize(unsigned int width, unsigned int height) {
	m_framebuffer->resize(width, height);
}

void PostProcessingLayer::onAfterRender() {
	Game& game = Game::getInstance();
	Renderer& renderer = Renderer::getInstance();

	m_framebuffer->bind();

	renderer.setViewport(
		{0.0f, 0.0f}, {m_framebuffer->getWidth(), m_framebuffer->getHeight()});
	renderer.clear();

	Framebuffer* primaryFramebuffer = game.getFramebuffer();
	Texture(primaryFramebuffer->getColourTextureId()).bind(3);

	DrawCubeRequest request;
	request.Manual = true;

	if (Grayscale) {
		m_shaderGrayscale.bind();
		m_shaderGrayscale.setInt("u_Texture", 3);

		renderer.drawCube(request);
		m_framebuffer->copy(*primaryFramebuffer);

		m_framebuffer->bind();
		renderer.clear();
	}

	if (Invert) {
		m_shaderInvert.bind();
		m_shaderInvert.setInt("u_Texture", 3);

		renderer.drawCube(request);
		m_framebuffer->copy(*primaryFramebuffer);
	}

	m_framebuffer->unbind();
}

} // namespace MattEngine