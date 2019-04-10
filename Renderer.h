#pragma once
#include "Headers.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	GLFWwindow *getWindow();

	void Render(GLuint gShaderProgram, GLuint gVertexAttribute[], float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc);
	int CreateFrameBuffer();
	void initWindow(unsigned int w, unsigned int h);
	void SetViewport();

private:

	

	GLFWwindow *gWindow;

	unsigned int gFbo;
	unsigned int gFboTextureAttachments[2]; // first for colour, second for depth

	/*float gClearColour[3];
	float gUniformColour[3];
	GLint gUniformColourLoc;*/
};

