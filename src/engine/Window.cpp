#include "Window.h"

#include "Game.h"
#include "Log.h"
#include "Renderer.h"

#include <iostream>
#include <string>

namespace MattEngine {

void windowResizeCallback(GLFWwindow* window, int width, int height) {}

void mouseCallback(GLFWwindow* window, double x, double y) {
	Window& instance = Window::getInstance();
	instance.MouseX = x;
	instance.MouseY = y;
	instance.MouseMoved = true;
}

Window::Window(
	const unsigned int width, const unsigned int height, std::string title)
	: m_title(title) {
	s_instance = this;
	unsigned int glfwResult = glfwInit();
	MATTENGINE_ASSERT(
		glfwResult, "Could not initialise GLFW - Error Code: %d", glfwResult);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
	glfwSwapInterval(1);

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	MATTENGINE_ASSERT(m_window, "Could not create window.", NULL);

	glfwMakeContextCurrent(m_window);
	glfwSetCursorPosCallback(m_window, mouseCallback);

	unsigned int gladResult = gladLoadGL();
	MATTENGINE_ASSERT(
		gladResult, "Could not load OpenGL - Error Code: %d", gladResult);

	glfwSetWindowSizeCallback(m_window, windowResizeCallback);
}

bool Window::isOpen() const { return !glfwWindowShouldClose(m_window); }

void Window::update() {
	glfwSwapBuffers(m_window);
	glfwWaitEventsTimeout(0.007f);
}

const std::string& Window::getTitle() const { return m_title; }

void Window::setTitle(const std::string& title) {
	glfwSetWindowTitle(m_window, title.c_str());
	m_title = title;
}

bool Window::isKeyDown(const int keycode) const {
	auto state = glfwGetKey(m_window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

void Window::setMouseEnabled(bool enabled) {
	glfwSetInputMode(m_window, GLFW_CURSOR,
		enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

} // namespace MattEngine