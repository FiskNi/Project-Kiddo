#include "Window.h"

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

bool Window::_init()
{

	if (!glfwInit()) {
		std::cout << "Failed to init" << std::endl;
		return false;
	}
	m_Window = glfwCreateWindow(m_Height, m_Width, m_Title, NULL, NULL);
	if (!m_Window) {
		std::cout << "Failed to create glfw window" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetKeyCallback(m_Window, key_callback);

	if (glewInit() != GLEW_OK) {
		std::cout << "Glue init failure" << std::endl;
		return false;
	}
	return true;
}

Window::Window(const char * name, int width, int height)
{
	m_Title = name;
	m_Height = height;
	m_Width = width;

	if (!_init()) {
		//glfwTerminate();
	}
	for (int i = 0; i < MAX_KEYS; i++) {
		m_Keys[i] = false;
	}
}

Window::~Window()
{
	glfwTerminate();

}

bool Window::closed() const
{
	return glfwWindowShouldClose(m_Window) == 1;
}

void Window::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::isKeyPressed(unsigned int keycode) const
{
	if (keycode >= MAX_KEYS)
		//TODO: Log this!!!
		return false;
	return m_Keys[keycode];
}

void Window::update()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "OpenGL Error : " << error << std::endl;
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Keys[key] = action != GLFW_RELEASE;
}