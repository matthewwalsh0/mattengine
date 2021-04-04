#include "Renderer.h"

#include "Log.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

namespace MattEngine {
    
    void Renderer::init() {
        s_instance = this;
        
        GLuint vertexBuffer, indexBuffer, vertexArray;

        float squareVertices[5 * 4] = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);

        glEnableVertexAttribArray(1);
        unsigned int offset = 3 * sizeof(float);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(intptr_t) offset);

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_View;
            uniform mat4 u_Transform;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_View * u_Transform * vec4(a_Position, 1.0);	
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            uniform bool u_Gradient = false;
            uniform vec3 u_Colour;
            uniform vec3 u_ColourStart;

            in vec3 v_Position;

            void main()
            {
                if(!u_Gradient) {
                    color = vec4(u_Colour, 1.0);
                } else {
                    float multiplier = (v_Position.x * 0.5) + (v_Position.y * 0.5);
                    color = vec4(
                        u_ColourStart.x + ((u_Colour.x - u_ColourStart.x) * multiplier),
                        u_ColourStart.y + ((u_Colour.y - u_ColourStart.y) * multiplier),
                        u_ColourStart.z + ((u_Colour.z - u_ColourStart.z) * multiplier),
                        1.0);
                }
            }
        )";

        std::string vertexSrcTexture = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_TexCoord;

            uniform mat4 u_View;
            uniform mat4 u_Transform;

            out vec2 v_TexCoord;

            void main()
            {
                v_TexCoord = a_TexCoord;
                gl_Position = u_View * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrcTexture = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;

            uniform sampler2D u_Texture;
            uniform int u_TileCount;
            uniform float u_Alpha;
 
            void main()
            {
                color = texture(u_Texture, v_TexCoord * u_TileCount) * vec4(1.0, 1.0, 1.0, u_Alpha);
            }
        )";

        std::string fragmentSrcText = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;

            uniform sampler2D u_Texture;
            uniform vec3 u_TextColour;

            void main()
            {
                vec2 coord = vec2(v_TexCoord.x, 1.0 - v_TexCoord.y);
                vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, coord).r);
                color = vec4(vec3(1.0, 1.0, 1.0), 1.0) * sampled;
            }  
        )";

        m_shaderTexture = createShader(vertexSrcTexture, fragmentSrcTexture);
        m_shaderColour = createShader(vertexSrc, fragmentSrc);
        m_shaderText = createShader(vertexSrcTexture, fragmentSrcText);

        glBindVertexArray(vertexArray);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   

        FT_Library ft;
        FT_Error ftError = FT_Init_FreeType(&ft);
        MATTENGINE_ASSERT(!ftError, "Could not iniailise Freetype - Error Code: %d", ftError);
    }

    void Renderer::drawQuad(const float x, const float y, const float width, const float height, const glm::vec3 colour) const {
        glUseProgram(m_shaderColour);

        GLint gradientLocation = glGetUniformLocation(m_shaderColour, "u_Gradient");
        glUniform1i(gradientLocation, false);

        GLint colourLocation = glGetUniformLocation(m_shaderColour, "u_Colour");
        glUniform3f(colourLocation, colour.x, colour.y, colour.z);

        drawQuad(x, y, width, height, m_shaderColour);
    }

    void Renderer::drawQuadGradient(const float x, const float y, const float width, const float height, const glm::vec3 colourStart, const glm::vec3 colourEnd) const {
        glUseProgram(m_shaderColour);

        GLint gradientLocation = glGetUniformLocation(m_shaderColour, "u_Gradient");
        glUniform1i(gradientLocation, true);

        GLint colourStartLocation = glGetUniformLocation(m_shaderColour, "u_ColourStart");
        glUniform3f(colourStartLocation, colourStart.x, colourStart.y, colourStart.z);

        GLint colourEndLocation = glGetUniformLocation(m_shaderColour, "u_Colour");
        glUniform3f(colourEndLocation, colourEnd.x, colourEnd.y, colourEnd.z);

        drawQuad(x, y, width, height, m_shaderColour);
    }

    void Renderer::drawQuadTexture(const float x, const float y, const float width, const float height, const Texture& texture, const unsigned int tileCount, float alpha) const {
        glUseProgram(m_shaderTexture);
        texture.Bind();

        GLint textureLocation = glGetUniformLocation(m_shaderTexture, "u_Texture");
        glUniform1i(textureLocation, 0);

        GLint tileCountLocation = glGetUniformLocation(m_shaderTexture, "u_TileCount");
        glUniform1i(tileCountLocation, tileCount);

        GLint alphaLocation = glGetUniformLocation(m_shaderTexture, "u_Alpha");
        glUniform1f(alphaLocation, alpha);

        drawQuad(x, y, width, height, m_shaderTexture);
    }

    void Renderer::drawText(const float x, const float y, const float height, const std::string& text, const glm::vec3 colour) {
        FT_Set_Pixel_Sizes(m_face, 0, 30);
        float x2 = x;

        glUseProgram(m_shaderText);

        GLint textureLocation = glGetUniformLocation(m_shaderText, "u_Texture");
        glUniform1i(textureLocation, 0);

        GLint colourLocation = glGetUniformLocation(m_shaderText, "u_TextColour");
        glUniform3f(colourLocation, colour.r, colour.g, colour.b);

        for(char character : text) {
            auto textureIt = m_charTextures.find(character);
            auto aspectIt = m_charAspects.find(character);

            if(textureIt == m_charTextures.end()) {
                FT_Error charError = FT_Load_Char(m_face, character, FT_LOAD_RENDER);
                MATTENGINE_ASSERT(!charError, "Could not load character: %c - Error Code: %d", character, charError);

                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);

                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    m_face->glyph->bitmap.width,
                    m_face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    m_face->glyph->bitmap.buffer
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                unsigned int bitmapWidth = m_face->glyph->bitmap.width;
                unsigned int bitmapHeight = m_face->glyph->bitmap.rows;
                float aspect = bitmapWidth == 0 ? 0.8 : bitmapWidth / (float) bitmapHeight;

                m_charTextures[character] = texture;
                m_charAspects[character] = aspect;

                textureIt = m_charTextures.find(character);
                aspectIt = m_charAspects.find(character);
            } else {
                glBindTexture(GL_TEXTURE_2D, textureIt->second);
            }

            float width = height * aspectIt->second;

            drawQuad(x2, y, width, height, m_shaderText);
            x2 += width;
        }
    }

    void Renderer::drawQuad(const float x, const float y, const float width, const float height, const unsigned int shaderId) const {
        GLint viewLocation = glGetUniformLocation(shaderId , "u_View");
        glm::mat4 view = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    
        GLint transformLocation = glGetUniformLocation(shaderId, "u_Transform");
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 1.0f));
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x, 1.0f - y - height, 0.0f)) * scale;
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    const unsigned int Renderer::createShader(const std::string& vertexSrc, const std::string& fragmentSrc) const {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, 0);
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        MATTENGINE_ASSERT(isCompiled == GL_TRUE, "Could not compile vertex shader.", NULL);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        MATTENGINE_ASSERT(isCompiled == GL_TRUE, "Could not compile fragment shader.", NULL);

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        MATTENGINE_ASSERT(isLinked == GL_TRUE, "Could not link shader.", NULL);

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);

        return program;
    }
}