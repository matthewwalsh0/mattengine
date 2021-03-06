#ifndef ENTITY_H
#define ENTITY_H

#include "DeleteComponent.h"
#include "TagComponent.h"
#include "Window.h"

#include "entt.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <string>

const float BALL_DIAMETER = 0.02f;
const float PADDLE_WIDTH = 0.3f;
const float PADDLE_HEIGHT = 0.025f;
const float PADDLE_MARGIN = 0.05f;

namespace MattEngine {

class Entity {
private:
	entt::registry* m_registry;
	entt::entity m_entity;
	bool m_empty = true;

public:
	Entity() {}
	Entity(entt::entity entityId, entt::registry* registry)
		: m_entity(entityId), m_registry(registry), m_empty(false) {}

	template <typename T> T& getComponent() {
		return m_registry->get<T>(m_entity);
	}

	template <typename T, typename... Args> T& addComponent(Args&&... args) {
		return m_registry->emplace<T>(m_entity, std::forward<Args>(args)...);
	}

	template <typename T> void removeComponent() {
		return m_registry->remove<T>(m_entity);
	}

	template <typename T> bool hasComponent() {
		return m_registry->has<T>(m_entity);
	}

	operator bool() const {
		entt::entity empty;
		return m_registry != nullptr && !m_empty;
	}

	bool operator==(const Entity& c2) { return m_entity == c2.m_entity; }
};
} // namespace MattEngine

#endif