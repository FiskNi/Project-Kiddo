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
#include "Scene.h"
#include "Menu.h"
#define MAX_KEYS 1024

class GameEngine
{
private:
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

	void CompileRoomData();


	// Main Menu & Pause Menu
	Menu mainMenu;
	void CompileMenuData(ACTIVEMENU activeMenu);
	ButtonVtx* mainMenuVertexData;
	ButtonVtx* pauseMenuVertexData;
	ButtonVtx* collectibleMenuVertexData;
	ButtonVtx* htpMenuVertexData;

	bool printMouseClickOnce;
	bool switchCursorOnce;

	float collMenuClearColour[3]{0.25f, 0.15f, 0.25f};

	irrklang::ISoundEngine* musicEngine;
	irrklang::ISoundEngine* soundEffectEngine;

public:
	GameEngine();
	~GameEngine();

	void Run();
	void ImGuiInit();
	void UpdateImGui(bool &renderDepth);
	static bool isKeyPressed(int keycode);
};

