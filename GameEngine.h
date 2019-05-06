#pragma once
#include "Headers.h"
#include "Renderer.h"
#include "ShadowMap.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Character.h"
#include "Light.h"
#include "Material.h"
#include "Entity.h"
#include "DirectionalLight.h"
#include "puzzleNode.h"
#include "Scene.h"
#define MAX_KEYS 1024
class GameEngine
{
private:
	static bool m_Keys[MAX_KEYS];
	// Full screen quad stuff
	// Might be moved later
	GLuint gVertexBufferFS = 0;
	GLuint gVertexAttributeFS = 0;
	GLuint gShaderProgramFS = 0;

	// ImGui debug content
	// Reference for creating custom debug UI
	float gFloat = 0;
	float gClearColour[3]{};
	glm::mat4 gRotate2D;


	// Main window and renderer object
	Renderer mainRenderer;
	vertexPolygon* mainSceneVertexData;

	int vertexCount;
	int meshCount;

	// Main scene
	Scene mainScene;
	friend void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	GameEngine();
	~GameEngine();

	void Run();
	void ImGuiInit();
	void UpdateImGui(bool &renderDepth);
	static bool isKeyPressed(int keycode);
};

