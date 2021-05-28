#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glm/glm.hpp>

namespace MattEngine {

class Framebuffer {
public:
	Framebuffer(const unsigned int width, const unsigned int height,
		bool hasColourAttachment = true, bool hasDepthAttachment = true,
		unsigned int samples = 0);
	void resize(const unsigned int width, const unsigned int height);
	void setSamples(const unsigned int samples);
	void bind() const;
	void unbind() const;
	unsigned int getColourTextureId() { return m_colourTextureId; }
	unsigned int getDepthTextureId() { return m_depthTextureId; }
	unsigned int getWidth() { return m_width; }
	unsigned int getHeight() { return m_height; }
	const glm::vec2 getSize() { return glm::vec2(m_width, m_height); }
	unsigned int getSamples() { return m_samples; }
	void copy(Framebuffer& target);
	void copyToScreen();

private:
	void invalidate();

private:
	unsigned int m_width, m_height;
	bool m_hasColourAttachment = true;
	bool m_hasDepthAttachment = true;
	unsigned int m_framebufferId = 0;
	unsigned int m_colourTextureId;
	unsigned int m_depthTextureId;
	unsigned int m_samples;
};

} // namespace MattEngine

#endif
