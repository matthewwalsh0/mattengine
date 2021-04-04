#ifndef SCRIPT_COMPONENT_H
#define SCRIPT_COMPONENT_H

#include "Entity.h"

#include <map>

using namespace MattEngine;

class EntityScript {
    public:
        Entity Entity;
    public:
        virtual void onCreate() {}
        virtual void onUpdate(float deltaTime, Renderer& renderer, Window& window) {}
        virtual ~EntityScript() {};

        inline MattEngine::Entity& getEntity() {
            return Entity;
        }
};

struct ScriptComponent {
    std::unique_ptr<EntityScript> Script;

    template<typename T, typename... Args>
    void init(Args&&... args) {
        Script = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    T& getScript() {
        return (T&) *Script;
    }
};

#endif
