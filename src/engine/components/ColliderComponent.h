#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "Physics.h"
#include "Entity.h"
#include "PhysicsComponent.h"
#include "TagComponent.h"

#include <glm/glm.hpp>

namespace MattEngine {

    class CollisionHandler {
        public:
        virtual void onCollision(CollisionType collisionType, Entity& staticEntity, Entity& movingEntity) = 0;
        virtual std::shared_ptr<CollisionHandler> clone() = 0;
        virtual ~CollisionHandler() {};
    };

    class DefaultCollisionHandler : public CollisionHandler {
        public:

        std::shared_ptr<CollisionHandler> clone() override { 
            return std::make_shared<DefaultCollisionHandler>();
        }

        void onCollision(CollisionType collisionType, Entity& staticEntity, Entity& movingEntity) override {

            TagComponent& movingTag = movingEntity.getComponent<TagComponent>();

            if(movingTag.Tag == "PowerUp") return;

            glm::vec2& velocity = movingEntity.getComponent<PhysicsComponent>().Velocity;

            if(collisionType == CollisionType::HORIZONTAL)
                velocity.x *= -1;
            else if(collisionType == CollisionType::VERTICAL)
                velocity.y *= -1;
        }
    };

    struct ColliderComponent {
        std::shared_ptr<CollisionHandler> Handler;

        ColliderComponent() {}

        ColliderComponent(const ColliderComponent& other) {
            if(other.Handler) {
                Handler = other.Handler->clone();
            }
        }

        template<typename T, typename... Args>
        void init(Args&&... args) {
            Handler = std::make_shared<T>(std::forward<Args>(args)...);
        }

        void withDefaultHandler() {
            Handler = std::make_shared<DefaultCollisionHandler>();
        }

        void withNoHandler() {
        }
    };

}

#endif
