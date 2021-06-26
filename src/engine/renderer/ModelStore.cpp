#include "ModelStore.h"

using namespace MattEngine;

Model& ModelStore::getModel(const std::string& path) {
	auto result = m_modelsByPath.find(path);

	if (result != m_modelsByPath.end()) {
		return result->second;
	}

	m_modelsByPath.emplace(path, path);

	return m_modelsByPath.find(path)->second;
}