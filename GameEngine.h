#pragma once
#include "Headers.h"
#include "Renderer.h"
#include "ShadowMap.h"
#include "Primitive.h"
#include "ShaderHandler.h"
#include "Camera.h"
#include "Character.h"
#include "Light.h"
#include "Material.h"

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void Run();

	void LoadContent();

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

	//*** Game content ***//
	// Main window and renderer object
	Renderer mainRenderer;

	// Shaders
	ShaderHandler basicShader;
	ShaderHandler fsqShader;
	ShaderHandler gShaderSM;
	
	// Shadowmap
	ShadowMap shadowMap;

	// Primitive mesh objects
	Primitive cubePrimitive;
	Primitive groundPlane;

	// Object list for the render queue
	std::vector<Primitive> objects;
	std::vector<vertexPolygon> renderObjectQueue;
	// Materials
	Material cubeMat;
	Material planeMat;

	// Main camera
	Camera mainCamera;


};

