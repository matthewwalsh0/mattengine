#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

namespace MattEngine {

class Framebuffer {
public:
	Framebuffer(const unsigned int width, const unsigned int height);
	void resize(const unsigned int width, const unsigned int height);
	void bind() const;
	void unbind() const;
	unsigned int getColourTextureId() { return m_colourTextureId; }

private:
	void invalidate();

private:
	int m_width, m_height;
	unsigned int m_framebufferId;
	unsigned int m_colourTextureId;
	unsigned int m_depthTextureId;
};

} // namespace MattEngine

#endif
