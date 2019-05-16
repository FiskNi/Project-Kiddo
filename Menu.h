#pragma once

#include "Headers.h"
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"

#include "Mesh.h"
#include "Material.h"
#include "MenuButton.h"
//#include "Renderer.h"

//#define PLAYING 1
//#define PAUSE 2


class Menu {
private:
	bool setUserPointer = false;

	GAMESTATE state = MAINMENU;

	GLuint pauseOverlayTexture;
	GLuint loadingTexture;
	// Make this an array of textures?
	GLuint buttonTextureBase;

	// Mesh vectors if we want a 3D scene to the menu
	//std::vector<Mesh> meshes;
	//std::vector<Mesh> menuMeshes;

	// Menu Button objects
	std::vector<MenuButton> menuButtons;

	// These are used to calculate the offset which is sent into the MenuButton constructor (Maybe only send in nr of buttons and calc in MenuButton?)
	int nrOfMenuButtons;
	const float BUTTON_OFFSET = 0.05f;
	const float buttonHeight = 0.3f;

	// The total number of vertices for the menu
	int vertexCountTotal;

	bool isMenuRunning;
	bool isLoading;

	// camera for the menu, if we want a 3D scene
	//Camera* menuCamera;

public:
	Menu();
	~Menu();

	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);

	void CreateMainMenu();
	void MenuUpdate(GLFWwindow* renderWindow, float deltaTime);
	void CreateMenuTexture(std::string path, GLuint *texture);


	//void CreateMenuQuad();

	//void CreateMainMenuRoom(std::vector<Material> materials, Loader * aLoader, int state);

	//void CompileMeshData();

	//void LoadEntities(std::vector<Material> materials, Loader * level);

	//void RenderLoading(Shader gShaderProgram);

	GLuint GetPauseOverlay() const { return pauseOverlayTexture; }
	GLuint GetLoadingTexture() const { return loadingTexture; }
	GLuint GetButtonTexture() const { return buttonTextureBase; }
	int GetVertexCountTotal() const { return vertexCountTotal; }
	std::vector<ButtonVtx> GetButtonVertices(int idx) const { return menuButtons[idx].GetButtonVertices(); }
	std::vector<MenuButton> GetMenuButtons() const { return menuButtons; }
	float GetCurrentOffset() const { return (nrOfMenuButtons-1) * (buttonHeight + BUTTON_OFFSET); }
	int GetNrOfMenuButtons() const { return nrOfMenuButtons; }

	bool GetIsMenuRunning() const { return isMenuRunning; }
	void SetIsMenuRunning(bool tf) { this->isMenuRunning = tf; }

	GAMESTATE GetCurrentState() const { return state; }
	void SetState(GAMESTATE newState) { this->state = newState; }

	bool GetIsLoading() const { return isLoading; }
	void SetIsLoading(bool isLoading) { this->isLoading = isLoading; }
	
};

