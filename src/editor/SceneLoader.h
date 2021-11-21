#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "Scene.h"

#include <string>

namespace MattEngine {

class SceneLoader {
public:
	static std::shared_ptr<Scene> load(const std::string& path);
};

} // namespace MattEngine

#endif
