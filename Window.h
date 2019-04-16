#pragma once
#include <iostream>
#include <glew/include/GL/glew.h>
#include <glfw/include/GLFW/glfw3.h>
#define MAX_KEYS 1024
class Window
{
private:
	int m_Width, m_Height;
	const char *m_Title;
	GLFWwindow *m_Window;

	bool _init();

	bool m_Keys[MAX_KEYS];

	friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	Window(const char* name, int width, int height);
	~Window();

	bool closed() const;
	void clear() const;

	bool isKeyPressed(unsigned int keycode) const;

	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }

	void update();
};
