#pragma once
#include "Headers.h"
#include "Primitive.h"
#include "Camera.h"
#include "ShadowMap.h"
#include "Shader.h"
#include "Light.h"
#include "DirLight.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	GLFWwindow *getWindow();

	void firstPassRenderTemp(Shader gShaderProgram, std::vector<Primitive> objects, float gClearColour[]);
	void secondPassRenderTemp(Shader gShaderProgram, ShadowMap SM);

	void prePassRender(Shader gShaderProgram, std::vector<Primitive> objects, Camera camera, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc, ShadowMap SM);

	void Render(Shader gShaderProgram, std::vector<Primitive> objects, Camera camera, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc, ShadowMap SM, Light lightArr[], DirLight aLight);

	int CreateFrameBuffer();
	void initWindow(unsigned int w, unsigned int h);
	void SetViewport();
	void CreateModelMatrix(glm::vec3 translation, float rotation, GLuint shaderProg);
	void passTextureData(GLuint TextureUnit, GLuint texID, GLuint shaderProg,
		GLchar* uniformName, int index);

private:

	GLFWwindow *gWindow;

	unsigned int gFbo;
	// Color and depth Render-to-texture for the fullscreen quad
	unsigned int gFboTextureAttachments[2];

	glm::mat4 MODEL_MAT;
	glm::mat4 VIEW_MAT;
	glm::mat4 PROJ_MAT;

};

