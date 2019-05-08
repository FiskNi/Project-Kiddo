#pragma once

#include "Headers.h"

#define PLAYING 1
#define PAUSE 2

class Menu {
private:
	GLuint pauseOverlayTexture;

public:
	Menu();
	~Menu();

	void CompileMenuMeshData();
	void CreatePauseOverlayTexture(std::string &path);

	GLuint* GetPauseOverlay() { return &pauseOverlayTexture; }
};