#pragma once
#include "Headers.h"
#include "Renderer.h"
#include "ShadowMap.h"
#include "Primitive.h"
#include "Shader.h"
#include "Camera.h"
#include "Character.h"
#include "Light.h"
#include "Material.h"
#include "Entity.h"
#include "DirLight.h"
#include "puzzleNode.h"

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void Run();

	void updateContent(float deltaTime);

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

	float gOffsetX = 0.0f;
	float gIncrement = 0.0f;
	float gRotateZ = 0.0f;

	glm::mat4 gRotate2D;

	//*** Game content ***//
	// Main window and renderer object
	Renderer mainRenderer;

	// Shaders
	Shader basicShader;
	Shader fsqShader;
	Shader gShaderSM;
	
	// Shadowmap
	ShadowMap shadowMap;

	// Object list for the render queue
	std::vector<Primitive> objects;

	// Materials are stored in a vector
	std::vector<Material> materials;

	// Main camera
	Camera mainCamera;

	// Lights
	std::vector<Light> lights;
	DirLight aDirLight;

	// Entity
	std::vector<Entity> entities;
	int entityIndex[256];

	std::vector<puzzleNode> nodes;
	int nodeIndex[20];

	// Character
	Character playerCharacter;
	int playerIndex;
};

