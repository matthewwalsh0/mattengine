#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

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
	GLFWwindow* getInternalWindow() { return m_window; }

public:
	inline static Window& getInstance() { return *s_instance; }

public:
	float MouseX = 0.0f;
	float MouseY = 0.0f;
	float MouseMoved = false;

private:
	GLFWwindow* m_window;
	std::string m_title;

private:
	inline static Window* s_instance;
};
} // namespace MattEngine

#endif