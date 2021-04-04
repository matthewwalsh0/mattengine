#include "Window.h"

#include "Log.h"

#include <string>
#include <iostream>

namespace MattEngine {

    void windowResizeCallback(GLFWwindow* window, int width, int height)
    {
        bool landscape = width >= height;
        int viewportWidth = landscape ? height : width;
        int viewportHeight = landscape ? height : width;
        int widthMargin = landscape ? (width - height) / 2 : 0;
        int heightMargin = landscape ? 0 : (height - width) / 2;

        glViewport(widthMargin, heightMargin, viewportWidth, viewportHeight);
    }
    
    Window::Window(const unsigned int width, const unsigned int height, std::string title)
        : m_title(title) {
            s_instance = this;
        unsigned int glfwResult = glfwInit();
        MATTENGINE_ASSERT(glfwResult, "Could not initialise GLFW - Error Code: %d", glfwResult);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
        glfwSwapInterval(1);

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        MATTENGINE_ASSERT(m_window, "Could not create window.", NULL);

        glfwMakeContextCurrent(m_window);

        unsigned int gladResult = gladLoadGL();
        MATTENGINE_ASSERT(gladResult, "Could not load OpenGL - Error Code: %d", gladResult);

        MATTENGINE_LOG("OpenGL:", NULL);
        MATTENGINE_LOG("  Vendor: %s", glGetString(GL_VENDOR));
        MATTENGINE_LOG("  Renderer: %s", glGetString(GL_RENDERER));
        MATTENGINE_LOG("  Version: %s", glGetString(GL_VERSION));

        glfwSetWindowSizeCallback(m_window, windowResizeCallback);
    }
    
    bool Window::isOpen() const {
        return !glfwWindowShouldClose(m_window);
    }

    void Window::update() {
        glfwSwapBuffers(m_window);
        glfwWaitEventsTimeout(0.007f);
    }

    const std::string& Window::getTitle() const {
        return m_title;
    }

    void Window::setTitle(const std::string& title) {
        glfwSetWindowTitle(m_window, title.c_str());
        m_title = title;
    }

    bool Window::isKeyDown(const int keycode) const {
        auto state = glfwGetKey(m_window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
}