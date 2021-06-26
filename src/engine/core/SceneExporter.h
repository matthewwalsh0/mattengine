#ifndef SCENE_EXPORTER_H
#define SCENE_EXPORTER_H

#include "Scene.h"

#include <string>

namespace MattEngine {

class SceneExporter {
public:
	static void save(Scene& scene, const std::string& path);
	static void load(const std::string& path);
};

} // namespace MattEngine

#endif
