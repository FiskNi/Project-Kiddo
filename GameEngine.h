#pragma once
#include "Headers.h"
#include "Renderer.h"
#include "ShaderHandler.h"
#include "CreatePrimitives.h"
#include "Camera.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void Run();

	void firstPassRenderTemp(GLuint gShaderProgram, GLuint gVertexAttribute);
	void secondPassRenderTemp();

	int CreateFrameBuffer();
	void CreateFullScreenQuad();

	//static void keyboard(GLFWwindow * window, int key, int scancode, int action, int mods);

private:
	// OpenGL uses unsigned integers to keep track of
	// created resources (shaders, vertices, textures, etc)
	// For simplicity, we make them global here, but it is
	// safe to put them in a class and pass around...

	// full screen quad stuff
	GLuint gVertexBufferFS = 0;
	GLuint gVertexAttributeFS = 0;
	GLuint gShaderProgramFS = 0;

	float gFloat = 0;

	float gClearColour[3]{};
	float gUniformColour[3]{};
	GLint gUniformColourLoc = -1;

	float gOffsetX = 0.0f;
	float gIncrement = 0.0f;
	float gRotateZ = 0.0f;

	glm::mat4 gRotate2D;
	// macro that returns "char*" with offset "i"
	// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"

	unsigned int gFbo;
	unsigned int gFboTextureAttachments[2]; // first for colour, second for depth

	// ImGui
	/*static float gTx[2];
	static float gRotate2Z;
	static float scale;
	static bool renderDepth;
	glm::mat4 identity;*/


	// Main window and renderer object
	Renderer mainRenderer;

	// Shader handles, handles all the shaders
	ShaderHandler shaderHandler;

	// Simple triangle for testing

};

