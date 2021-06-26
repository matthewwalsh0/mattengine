#ifndef MODEL_STORE_H
#define MODEL_STORE_H

#include "Model.h"

#include <map>
#include <string>

namespace MattEngine {

class ModelStore {
public:
	Model& getModel(const std::string& path);

public:
	static ModelStore& getInstance() {
		if (!s_instance) {
			s_instance = new ModelStore();
		}

		return *s_instance;
	}

private:
	std::map<std::string, Model> m_modelsByPath;

private:
	inline static ModelStore* s_instance = nullptr;
};

} // namespace MattEngine

#endif
