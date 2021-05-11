#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

namespace MattEngine {

class Framebuffer {
public:
	Framebuffer(const unsigned int width, const unsigned int height,
		bool hasColourAttachment = true, bool hasDepthAttachment = true);
	void resize(const unsigned int width, const unsigned int height);
	void bind() const;
	void unbind() const;
	unsigned int getColourTextureId() { return m_colourTextureId; }
	unsigned int getDepthTextureId() { return m_depthTextureId; }
	unsigned int getWidth() { return m_width; }
	unsigned int getHeight() { return m_height; }

private:
	void invalidate();

private:
	unsigned int m_width, m_height;
	bool m_hasColourAttachment = true;
	bool m_hasDepthAttachment = true;
	unsigned int m_framebufferId;
	unsigned int m_colourTextureId;
	unsigned int m_depthTextureId;
};

} // namespace MattEngine

#endif
