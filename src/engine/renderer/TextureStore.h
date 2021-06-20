#ifndef TEXTURE_STORE_H
#define TEXTURE_STORE_H

#include "Texture.h"

#include <map>
#include <string>

namespace MattEngine {

class TextureStore {
public:
	Texture& getTexture(const std::string& path);

public:
	static TextureStore& getInstance() {
		if (!s_instance) {
			s_instance = new TextureStore();
		}

		return *s_instance;
	}

private:
	std::map<std::string, Texture> m_texturesByPath;

private:
	inline static TextureStore* s_instance = nullptr;
};

} // namespace MattEngine

#endif
