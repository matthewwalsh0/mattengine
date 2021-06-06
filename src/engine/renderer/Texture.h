#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace MattEngine {

class Texture {
public:
	Texture(const std::string& path);
	Texture(const Texture& texture);
	Texture(const int width, const int height, const int channels, void* data);
	Texture(unsigned int textureId);
	Texture(void* data, int length);
	void bind(unsigned int slot = 0) const;

	const int getWidth() { return m_width; }
	const int getHeight() { return m_height; }

private:
	std::string m_path;
	int m_width, m_height;
	unsigned int m_textureId;
};

} // namespace MattEngine

#endif
