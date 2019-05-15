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
	GLuint pauseOverlayTexture;
	GLuint loadingTexture;

	GLuint buttonTextureBase;

	std::vector<Mesh> meshes;
	std::vector<Mesh> menuMeshes;

	std::vector<MenuButton> menuButtons;
	//std::vector<ButtonVtx> buttonVertices;

	int nrOfMenuButtons;
	const float BUTTON_OFFSET = 0.05f;
	const float buttonHeight = 0.3f;

	int vertexCountTotal;

	bool isMenuRunning;

	Camera* menuCamera;

public:
	Menu();
	~Menu();

	void CreateMainMenu();

	//void CompileMainMenuMeshData();
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
	float GetCurrentOffset() const { return nrOfMenuButtons * (buttonHeight + BUTTON_OFFSET); }
	int GetNrOfMenuButtons() const { return nrOfMenuButtons; }

	bool GetIsMenuRunning() const { return isMenuRunning; }
	void SetIsMenuRunning(bool tf) { this->isMenuRunning = tf; }
	
};