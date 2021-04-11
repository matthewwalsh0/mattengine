#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace MattEngine {

class Texture {
public:
	Texture(const std::string& path);
	Texture(const Texture& texture);
	Texture(const int width, const int height, const int channels, void* data);
	inline const int getWidth() { return m_width; }
	inline const int getHeight() { return m_height; }
	void bind() const;

private:
	std::string m_path;
	int m_width, m_height;
	unsigned int m_textureId;
};

} // namespace MattEngine

#endif
