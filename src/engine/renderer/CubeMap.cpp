#include "CubeMap.h"

#include "Log.h"

#include "stb_image.h"
#include <glad/glad.h>

#include <string>
#include <vector>

namespace MattEngine {

CubeMap::CubeMap(const std::string& directory) {
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

	std::vector<std::string> files{"right.jpg", "left.jpg", "top.jpg",
		"bottom.jpg", "front.jpg", "back.jpg"};

	int width, height, channels;
	unsigned char* data;

	stbi_set_flip_vertically_on_load(0);

	for (unsigned int i = 0; i < files.size(); i++) {
		std::string path = directory + "/" + files[i];

		data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		MATTENGINE_ASSERT(
			data, "Cannot load cube map face from file: %s", path.c_str());

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
			height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap::bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId); }

void CubeMap::unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

} // namespace MattEngine