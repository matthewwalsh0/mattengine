#include "Framebuffer.h"

#include "Log.h"

#include <glad/glad.h>

using namespace MattEngine;

Framebuffer::Framebuffer(const unsigned int width, const unsigned int height) {
	m_width = width;
	m_height = height;
	invalidate();
}

void Framebuffer::resize(const unsigned int width, const unsigned int height) {
	m_width = width;
	m_height = height;
	invalidate();
}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
}

void Framebuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::invalidate() {
	glGenFramebuffers(1, &m_framebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);

	glGenTextures(1, &m_colourTextureId);
	glBindTexture(GL_TEXTURE_2D, m_colourTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		m_colourTextureId, 0);

	glGenTextures(1, &m_depthTextureId);
	glBindTexture(GL_TEXTURE_2D, m_depthTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
		m_depthTextureId, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		MATTENGINE_ASSERT(
			glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Framebuffer is not valid.", NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}