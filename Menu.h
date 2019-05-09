#pragma once

#include "Headers.h"

//#define PLAYING 1
//#define PAUSE 2

enum GAMESTATE {
	PAUSED,
	PLAYING,
	MAINMENU
};

class Menu {
private:
	GLuint pauseOverlayTexture;

public:
	Menu();
	~Menu();

	//void CompileMainMenuMeshData();
	void CreatePauseOverlayTexture(std::string path);

	GLuint GetPauseOverlay() { return pauseOverlayTexture; }
};