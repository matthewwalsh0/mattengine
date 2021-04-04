#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include <glm/glm.hpp>

#include <string>

struct TextComponent {
    std::string Text;
    glm::vec2 Position;
    float Height;

    TextComponent(const std::string& text, const glm::vec2& position, float height) :
        Text(text), Position(position), Height(height) {}
};

#endif