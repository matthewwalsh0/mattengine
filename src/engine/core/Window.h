#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

namespace MattEngine {

class Window {
public:
	Window(
		const unsigned int width, const unsigned int height, std::string title);
	bool isOpen() const;
	void update();
	const std::string& getTitle() const;
	void setTitle(const std::string& title);
	bool isKeyDown(const int keycode) const;
	void setMouseEnabled(bool enabled);
	const glm::vec2& getSize();

	GLFWwindow* getInternalWindow() const { return m_window; }
	const glm::vec2& getMousePosition() const { return m_mousePosition; }
	const bool getMouseLeftButtonDown() const { return m_mouseLeftDown; }
	const bool getMouseRightButtonDown() const { return m_mouseRightDown; }
	const bool hasMouseMoved() const { return m_mouseMoved; }
	void setFocused(bool value) { m_focused = value; }
	void resetMouseTracking() { m_mouseMoved = false; }

public:
	inline static Window& getInstance() { return *s_instance; }

private:
	GLFWwindow* m_window;
	std::string m_title;
	glm::vec2 m_mousePosition;
	glm::vec2 m_size = {0.0f, 0.0f};
	bool m_mouseMoved = false;
	bool m_mouseLeftDown = false;
	bool m_mouseRightDown = false;
	bool m_focused = true;

private:
	inline static Window* s_instance;

	friend void windowResizeCallback(GLFWwindow* window, int width, int height);
	friend void mouseCursorCallback(GLFWwindow* window, double x, double y);
	friend void mouseButtonCallback(
		GLFWwindow* window, int button, int actions, int mods);
};
} // namespace MattEngine

#endif