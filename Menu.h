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

	GLuint pauseOverlayTexture;
	GLuint loadingTexture;
	//GLuint pauseButtonTexture;
	GLuint pbt0;
	GLuint pbt1;
	GLuint pbt2;
	GLuint pbt3;

	GLuint buttonTextureBase;
	std::vector<GLuint> buttonTextures;
	std::vector<GLuint> pauseButtonTextures;

	GLuint backgroundTexture;

	// Menu Button objects
	std::vector<MenuButton> mainButtons;
	std::vector<MenuButton> pauseButtons;

	// These are used to calculate the offset which is sent into the MenuButton constructor (Maybe only send in nr of buttons and calc in MenuButton?)
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

	std::vector<ButtonVtx> backgroundQuad;
	std::vector<Collectible> collected;


public:
	Menu();
	~Menu();

	void CreateMainMenu();
	void MenuUpdate(GLFWwindow* renderWindow, float deltaTime);
	void CreateMenuTexture(std::string path, GLuint *texture);

	bool CheckCollision(float x, float y);

	void CreateMainMenuButtons();
	void CreateBackgroundQuad();

	// Get Textures ( some are temporary right now ) 
	GLuint GetPauseOverlay() const						{ return pauseOverlayTexture; }
	GLuint GetLoadingTexture() const					{ return loadingTexture; }
	GLuint GetButtonTexture() const						{ return buttonTextureBase; }
	//GLuint GetPauseButtonTexture() const				{ return pauseButtonTexture; }
	GLuint GetBackgroundTexture() const					{ return backgroundTexture; }
	std::vector<GLuint> GetButtonTextures() const		{ return buttonTextures; }
	std::vector<GLuint> GetPauseButtonTextures() const	{ return pauseButtonTextures; }

	int GetVertexCountMainTotal() const									{ return vertexCountMainTotal; }
	std::vector<ButtonVtx> GetMainMenuButtonVertices(int idx) const		{ return mainButtons[idx].GetButtonVertices(); }
	std::vector<MenuButton> GetMainMenuButtons() const					{ return mainButtons; }
	int GetVertexCountPauseTotal() const								{ return vertexCountPauseTotal; }
	std::vector<ButtonVtx> GetPauseMenuButtonVertices(int idx) const	{ return pauseButtons[idx].GetButtonVertices(); }
	std::vector<MenuButton> GetPauseMenuButtons() const					{ return pauseButtons; }

	float GetCurrentOffset() const					{ return (nrOfMainButtons - 1) * (buttonHeight + BUTTON_OFFSET); }
	float GetCurrentOffsetPause() const				{ return (nrOfPauseButtons - 1) * (buttonHeight + BUTTON_OFFSET); }
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

