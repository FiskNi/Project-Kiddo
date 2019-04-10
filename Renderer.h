#pragma once
#include "Headers.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	GLFWwindow *getWindow();

	void Render(GLuint gShaderProgram, GLuint gVertexAttribute, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc);

private:
	void initWindow(unsigned int w, unsigned int h);

	GLFWwindow *gWindow;

	/*float gClearColour[3];
	float gUniformColour[3];
	GLint gUniformColourLoc;*/
};

