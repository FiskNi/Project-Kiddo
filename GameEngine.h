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
	ButtonVtx* mainMenuVertexData;
	void CompileMainMenuData();
	ButtonVtx* pauseMenuVertexData;
	void CompilePauseMenuData();
	ButtonVtx* collectibleMenuVertexData;
	void CompileCollectibleMenuData();
	bool printMouseClickOnce;
	bool switchCursorOnce;

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

