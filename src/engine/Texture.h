#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace MattEngine {

class Texture {
public:
	Texture(const std::string& path);
	Texture(const Texture& texture);
	inline const int GetWidth() { return m_width; }
	inline const int GetHeight() { return m_height; }
	void Bind() const;

private:
	std::string m_path;
	int m_width, m_height;
	unsigned int m_textureId;
};

} // namespace MattEngine

#endif
