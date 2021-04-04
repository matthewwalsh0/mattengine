#include "Game.h"
#include "TransformComponent.h"
#include "ColourComponent.h"

class DemoScene : public MattEngine::Scene {
    private:
        float SPEED = 0.4f;

    private:
        TransformComponent* m_transform;
        float m_deltaX = SPEED;

    public:

    void onInit() override {
        MattEngine::Entity entity = createEntity();
        entity.addComponent<TransformComponent>(glm::vec2(0.3f, 0.3f), glm ::vec2(0.4f, 0.4f));
        entity.addComponent<ColourComponent>(glm::vec3(1.0f, 0.0f, 0.0f));

        m_transform = &entity.getComponent<TransformComponent>();
    }

    void onUpdate(float deltaTime) override {
        float currentX = m_transform->Position.x;

        if(currentX > 0.6f || currentX < 0.0f) {
            m_deltaX *= -1.0f;
        }

        m_transform->Position.x += m_deltaX * deltaTime;
    }
};

class Demo : public MattEngine::Game {
    public:
        Demo(MattEngine::Window& window) : Game(window) {}

        void onInit() override {
            loadScene<DemoScene>();
        }
};

int main(int argc, char **argv)
{
    MattEngine::Window window(640, 480, "MattEngine Demo");
    Demo(window).start();
    return 0;
}