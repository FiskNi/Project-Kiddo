#pragma once

#include "Headers.h"
#include "Material.h"
#include "MenuButton.h"
#include "Collectible.h"

enum ACTIVEMENU 
{
	MAINACTIVE = 0,
	PAUSEACTIVE = 1
};

class Menu 
{
private:

	ACTIVEMENU activeMenu = MAINACTIVE;

	//GLuint pauseOverlayTexture;
	GLuint loadingTexture;
	GLuint pauseBackgroundTexture;
	GLuint backgroundTexture;

	GLuint buttonTextureBase;
	std::vector<GLuint> buttonTextures;
	std::vector<GLuint> pauseButtonTextures;

	// Menu Button objects
	std::vector<MenuButton> mainButtons;
	std::vector<MenuButton> pauseButtons;
	std::vector<ButtonVtx> backgroundQuad;

	// These are used to calculate the offset for buttons
	int nrOfMainButtons;
	int nrOfPauseButtons;
	const float BUTTON_OFFSET	= 0.05f;
	const float buttonHeight	= 0.3f;

	// The total number of vertices for the menu
	int vertexCountMainTotal;
	int vertexCountPauseTotal;

	bool isMenuRunning;
	bool isLoading;

	bool isButtonHit;
	int currentButtonHit;
	bool buttonActionExecuted;

	GAMESTATE updateState;

	bool printMouseClickOnce;

	std::vector<Collectible> collected;


public:
	Menu();
	~Menu();

	void CreateMainMenu();
	void MenuUpdate(GLFWwindow* renderWindow, float deltaTime);
	void CreateMenuTexture(std::string path, GLuint *texture);

	bool CheckCollision(float x, float y);

	void CreateMainMenuButtons();
	void CreatePauseMenuButtons();
	void CreateBackgroundQuad();

	// Get Textures
	GLuint GetLoadingTexture() const					{ return loadingTexture; }
	std::vector<GLuint> GetButtonTextures() const		{ return buttonTextures; }
	std::vector<GLuint> GetPauseButtonTextures() const	{ return pauseButtonTextures; }

	int GetVertexCountMainTotal() const									{ return vertexCountMainTotal; }
	std::vector<ButtonVtx> GetMainMenuButtonVertices(int idx) const		{ return mainButtons[idx].GetButtonVertices(); }
	std::vector<MenuButton> GetMainMenuButtons() const					{ return mainButtons; }
	int GetVertexCountPauseTotal() const								{ return vertexCountPauseTotal; }
	std::vector<ButtonVtx> GetPauseMenuButtonVertices(int idx) const	{ return pauseButtons[idx].GetButtonVertices(); }
	std::vector<MenuButton> GetPauseMenuButtons() const					{ return pauseButtons; }

	// Offset is for stacked menus, which will be used if we make a collectible menu
	float GetCurrentOffset(int nrOfButtons) const	{ return (nrOfButtons - 1) * (buttonHeight + BUTTON_OFFSET); }
	int GetNrOfMenuButtons() const					{ return nrOfMainButtons; }
	int GetNrOfPauseButtons() const					{ return nrOfPauseButtons; }

	ButtonVtx GetBackgroundVertices(int idx) const	{ return backgroundQuad[idx]; }

	bool GetIsMenuRunning() const					{ return isMenuRunning; }
	void SetIsMenuRunning(bool tf)					{ this->isMenuRunning = tf; }

	ACTIVEMENU GetActiveMenu() const				{ return activeMenu; }
	void SetActiveMenu(ACTIVEMENU nm)				{ this->activeMenu = nm; }

	int GetLastClickedButton() const				{ return currentButtonHit; }
	bool GetHasButtonActionExecuted() const			{ return buttonActionExecuted; }
	void SetButtonActionExecuted(bool tf)			{ this->buttonActionExecuted = tf; }

	GAMESTATE GetUpdateState() const				{ return updateState; }

	bool GetIsLoading() const						{ return isLoading; }
	void SetIsLoading(bool isLoading)				{ this->isLoading = isLoading; }

	void SetCollected(std::vector<Collectible> coll);
	
};

