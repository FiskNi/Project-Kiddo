#pragma once
#include "Headers.h"
#include "CreatePrimitives.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	GLFWwindow *getWindow();

	void Render(GLuint gShaderProgram, std::vector<CreatePrimitive> objects, Camera camera, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc);
	int CreateFrameBuffer();
	void initWindow(unsigned int w, unsigned int h);
	void SetViewport();
	void CreateModelMatrix(glm::vec3 translation, float rotation, GLuint shaderProg);

private:

	

	GLFWwindow *gWindow;

	unsigned int gFbo;
	unsigned int gFboTextureAttachments[2]; // first for colour, second for depth

	glm::mat4 MODEL_MAT;
	glm::mat4 VIEW_MAT;
	glm::mat4 PROJ_MAT;

	/*float gClearColour[3];
	float gUniformColour[3];
	GLint gUniformColourLoc;*/
};

