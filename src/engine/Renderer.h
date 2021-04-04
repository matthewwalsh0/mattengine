#ifndef RENDERER_H
#define RENDERER_H

#include "Texture.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <map>

namespace MattEngine {

    class Renderer {
    public:
        void init();
        void drawQuad(const float x, const float y, const float width, const float height, const glm::vec3 colour) const;
        void drawQuadGradient(const float x, const float y, const float width, const float height, const glm::vec3 colourStart, const glm:: vec3 colourEnd) const;
        void drawQuadTexture(const float x, const float y, const float width, const float height, const Texture& texture, const unsigned int tileCount = 1, float alpha = 1.0f) const;
        void drawText(const float x, const float y, const float height, const std::string& text, const glm::vec3 colour);
    public:
        inline static Renderer& getInstance() {
            return *s_instance;
        }
    private:
        const unsigned int createShader(const std::string& vertexSrc, const std::string& fragmentSrc) const;
        void drawQuad(const float x, const float y, const float width, const float height, const unsigned int shaderId) const;
    private:
        unsigned int m_shaderColour;
        unsigned int m_shaderTexture;
        unsigned int m_shaderText;
        FT_Face m_face;
        std::map<char, unsigned int> m_charTextures;
        std::map<char, float> m_charAspects;
    private:
        inline static Renderer* s_instance;
    };
}

#endif