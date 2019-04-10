#include "Renderer.h"



Renderer::Renderer()
{
	initWindow(WIDTH, HEIGHT);
}


Renderer::~Renderer()
{

}

GLFWwindow* Renderer::getWindow()
{
	return gWindow;
}

void Renderer::Render(GLuint gShaderProgram, GLuint gVertexAttribute, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc)
{
	// set the color TO BE used (this does not clear the screen right away)
	glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], 1.0f);

	// use the color to clear the color buffer (clear the color buffer only)
	glClear(GL_COLOR_BUFFER_BIT);

	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram);

	glUniform3fv(gUniformColourLoc, 1, &gUniformColour[0]);

	// tell opengl we are going to use the VAO we described earlier
	glBindVertexArray(gVertexAttribute);

	// ask OpenGL to draw 3 vertices starting from index 0 in the vertex array 
	// currently bound (VAO), with current in-use shader. Use TOPOLOGY GL_TRIANGLES,
	// so for one triangle we need 3 vertices!
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::initWindow(unsigned int w, unsigned int h)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gWindow = glfwCreateWindow(w, h, "basic GLFW window", NULL, NULL);
	if (!gWindow) {
		fprintf(stderr, "error creating window\n");
		exit(-1);
	}
	glfwMakeContextCurrent(gWindow);
	glewExperimental = GL_TRUE;

	// mouse callback
	// glfwSetCursorPosCallback(gWindow, mouseMoved);

	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error GLEW: %s\n", glewGetErrorString(err));
	}
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	fprintf(stderr, "Renderer: %s\n", renderer);
	fprintf(stderr, "OpenGL version %s\n", version);

	// start up time
	// timerStart = glfwGetTime();
	glClearColor(0.9f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);
	glViewport(0, 0, w, h);

	return;
}