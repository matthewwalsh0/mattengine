#include "Scene.h"

#include "Entity.h"
#include "DeleteComponent.h"
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

    Entity Scene::getEntity(const std::string& tag) {
        entt::entity rawEntity;

        m_registry.view<TagComponent>().each(
            [&](const auto entity, TagComponent& currentTag) {
                if(currentTag.Tag == tag) {
                    rawEntity = entity;
                }
            }
        );

        return Entity(rawEntity, &m_registry);
    }

    std::vector<Entity> Scene::getEntities(const std::string& tag) {
        std::vector<Entity> matches;

        m_registry.view<TagComponent>().each(
            [&](const auto entity, TagComponent& currentTag) {
                if(currentTag.Tag == tag) {
                    matches.push_back(Entity(entity, &m_registry));
                }
            });

        return matches;
    }
}