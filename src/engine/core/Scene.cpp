#include "Scene.h"

#include "DeleteComponent.h"
#include "Entity.h"
#include "TagComponent.h"

namespace MattEngine {

Entity Scene::createEntity() {
	return Entity(m_registry.create(), &m_registry);
}

Entity Scene::createEntity(entt::entity rawEntity) {
	return Entity(rawEntity, &m_registry);
}

void Scene::deleteEntity(Entity& entity) {
	entity.addComponent<DeleteComponent>();
}

std::optional<Entity> Scene::getEntity(const std::string& tag) {
	std::optional<Entity> foundEntity;

	m_registry.view<TagComponent>().each(
		[&](const auto entity, TagComponent& currentTag) {
			if (currentTag.Tag == tag) {
				foundEntity = Entity(entity, &m_registry);
			}
		});

	return foundEntity;
}

std::vector<Entity> Scene::getEntities(const std::string& tag) {
	std::vector<Entity> matches;

	m_registry.view<TagComponent>().each(
		[&](const auto entity, TagComponent& currentTag) {
			if (currentTag.Tag == tag) {
				matches.push_back(Entity(entity, &m_registry));
			}
		});

	return matches;
}

std::vector<Entity> Scene::getAllEntities() {
	std::vector<Entity> entities;

	m_registry.view<TagComponent>().each(
		[&](const auto entity, TagComponent& currentTag) {
			entities.push_back(Entity(entity, &m_registry));
		});

	return entities;
}
} // namespace MattEngine