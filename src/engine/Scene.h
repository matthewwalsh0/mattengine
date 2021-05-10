#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
#include "Renderer.h"
#include "entt.hpp"

#include <string>

namespace MattEngine {

class Scene {
public:
	virtual void onInit(){};
	virtual void onBeforeUpdate(Renderer& renderer){};
	virtual void onUpdate(float deltaTime){};

public:
	inline static Scene& getCurrent() { return *s_currentScene; }

	inline static void setCurrent(std::shared_ptr<Scene> scene) {
		s_currentScene = scene;
	}

	inline static bool isSimulation() { return s_simulation; }

	inline static void setSimulation(bool state) { s_simulation = state; }

	virtual ~Scene() {}

public:
	Entity createEntity();
	Entity createEntity(entt::entity rawEntity);
	void deleteEntity(Entity& entity);
	Entity getEntity(const std::string& tag);
	std::vector<Entity> getEntities(const std::string& tag);
	std::vector<Entity> getAllEntities();
	inline entt::registry& getRegistry() { return m_registry; };

private:
	entt::registry m_registry;

private:
	inline static std::shared_ptr<Scene> s_currentScene;
	inline static bool s_simulation = false;
};
} // namespace MattEngine

#endif
