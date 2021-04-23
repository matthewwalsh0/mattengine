#include "Texture.h"

#include "Log.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace MattEngine {

namespace Utils {

static unsigned int generateTexture(
	const int width, const int height, const int channels, void* data) {

	unsigned int textureId;
	GLenum internalFormat = 0;
	GLenum dataFormat = 0;

	if (channels == 4) {
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	} else if (channels == 3) {
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	glGenTextures(1, &textureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(
		GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat,
		GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureId;
}

} // namespace Utils

Texture::Texture(const std::string& path) {
	int channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), &m_width, &m_height, &channels, 0);

	MATTENGINE_ASSERT(data, "Cannot load texture from file: %s", path.c_str());

	m_textureId = Utils::generateTexture(m_width, m_height, channels, data);

	stbi_image_free(data);
}

Texture::Texture(const Texture& texture) { m_textureId = texture.m_textureId; }

Texture::Texture(
	const int width, const int height, const int channels, void* data) {

	m_width = width;
	m_height = height;
	m_textureId = Utils::generateTexture(width, height, channels, data);
}

void Texture::bind() const {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}
} // namespace MattEngine