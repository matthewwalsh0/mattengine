#include "TextureStore.h"

using namespace MattEngine;

Texture& TextureStore::getTexture(const std::string& path) {
	auto result = m_texturesByPath.find(path);

	if (result != m_texturesByPath.end()) {
		return result->second;
	}

	m_texturesByPath.emplace(path, path);

	return m_texturesByPath.find(path)->second;
}