#include "Framebuffer.h"

#include "Log.h"

#include <glad/glad.h>

using namespace MattEngine;

Framebuffer::Framebuffer(const unsigned int width, const unsigned int height,
	bool hasColourAttachment, bool hasDepthAttachment, unsigned int samples)
	: m_width(width), m_height(height),
	  m_hasColourAttachment(hasColourAttachment),
	  m_hasDepthAttachment(hasDepthAttachment), m_samples(samples) {
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

void Framebuffer::copy(Framebuffer& target) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.m_framebufferId);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Framebuffer::invalidate() {
	glGenFramebuffers(1, &m_framebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);

	if (m_hasColourAttachment) {
		glGenTextures(1, &m_colourTextureId);

		if (m_samples == 0) {
			glBindTexture(GL_TEXTURE_2D, m_colourTextureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, m_colourTextureId, 0);
		} else {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colourTextureId);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples,
				GL_RGB, m_width, m_height, GL_TRUE);
			glTexParameteri(
				GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(
				GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D_MULTISAMPLE, m_colourTextureId, 0);
		}

	} else {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if (m_hasDepthAttachment) {
		glGenTextures(1, &m_depthTextureId);

		if (m_samples == 0) {
			glBindTexture(GL_TEXTURE_2D, m_depthTextureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width,
				m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_2D, m_depthTextureId, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(
				GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(
				GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			glTexParameterfv(
				GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		} else {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_depthTextureId);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples,
				GL_DEPTH_COMPONENT, m_width, m_height, GL_TRUE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_2D_MULTISAMPLE, m_depthTextureId, 0);

			glTexParameteri(
				GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(
				GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S,
				GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T,
				GL_CLAMP_TO_BORDER);
			float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			glTexParameterfv(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_BORDER_COLOR,
				borderColor);
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		MATTENGINE_ASSERT(
			glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Framebuffer is not valid.", NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}