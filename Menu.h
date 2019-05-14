#pragma once

#include "Headers.h"
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"

#include "Mesh.h"
#include "Material.h"
//#include "Renderer.h"

//#define PLAYING 1
//#define PAUSE 2


class Menu {
private:
	GLuint pauseOverlayTexture;
	GLuint loadingTexture;

	std::vector<Mesh> meshes;
	std::vector<Mesh> menuMeshes;

	Camera* menuCamera;

public:
	Menu();
	~Menu();

	//void CreateMainMenu();

	//void CompileMainMenuMeshData();
	void CreatePauseOverlayTexture(std::string path);
	void CreateLoadingTexture(std::string path);

	void CreateMainMenuRoom(std::vector<Material> materials, Loader * aLoader, int state);

	void CompileMeshData();

	void LoadEntities(std::vector<Material> materials, Loader * level);

	//void RenderLoading(Shader gShaderProgram);

	GLuint GetPauseOverlay() { return pauseOverlayTexture; }
	GLuint GetLoadingTexture() { return loadingTexture; }
};