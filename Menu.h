#pragma once

#include "Headers.h"
#include "Material.h"
#include "MenuButton.h"
#include "Collectible.h"

enum ACTIVEMENU
{
	MAINACTIVE = 0,
	PAUSEACTIVE = 1,
	COLLECTIBLEACTIVE = 2,
	HTPACTIVE = 3
};

class Menu 
{
private:

	ACTIVEMENU activeMenu = MAINACTIVE;

	//GLuint pauseOverlayTexture;
	GLuint loadingTexture;
	GLuint pauseBackgroundTexture;
	GLuint backgroundTexture;
	GLuint howToPlayTexture;
	GLuint collectibleMenuTexture;

	// Textures for dynamic Collectible Menu
	GLuint collHeaderTexture;
	GLuint collEmptyTexture;
	GLuint collTextures[9];
	GLuint collReturnTexture;

	GLuint buttonTextureBase;
	std::vector<GLuint> buttonTextures;
	std::vector<GLuint> pauseButtonTextures;
	std::vector<GLuint> collectibleTextures;
	std::vector<GLuint> htpTextures;

	// Menu Button objects
	std::vector<MenuButton> mainButtons;
	std::vector<MenuButton> pauseButtons;
	std::vector<MenuButton> collectibleButtons;
	std::vector<MenuButton> htpButtons;

	std::vector<ButtonVtx> backgroundQuad;
	std::vector<ButtonVtx> headerQuad;
	std::vector<ButtonVtx> returnQuad;

	// These are used to calculate the offset for buttons
	int nrOfMainButtons;
	int nrOfPauseButtons;
	int nrOfCollectibleButtons;
	int nrOfHtpButtons;
	const float BUTTON_OFFSET	= 0.0f;
	const float buttonWidth	= 0.165f;

	// The total number of vertices for the menu
	int vertexCountMainTotal;
	int vertexCountPauseTotal;
	int vertexCountCollectibleTotal;
	int vertexCountHtpTotal;

	bool isMenuRunning;
	bool isLoading;

	bool isButtonHit;
	int currentButtonHit;
	bool buttonActionExecuted;

	GAMESTATE updateState;

	bool printMouseClickOnce;

	std::vector<Collectible> collected;

	bool isHovering;

	irrklang::ISoundEngine* musicEngine;

public:

	Menu();
	~Menu();

	void CreateMainMenu();
	void MenuUpdate(GLFWwindow* renderWindow, float deltaTime);
	void CreateMenuTexture(std::string path, GLuint *texture);

	bool CheckCollision(float x, float y, bool isClicked);

	bool CheckButtonHovering(GLFWwindow * renderWindow);

	void CreateMainMenuButtons();
	void CreatePauseMenuButtons();
	void CreateCollectibleMenuButtons();
	void CreateHTPMenuButtons();

	// Function for creating textures for the dynamic collectible menu
	void CreateCollectibleTextures();

	void CreateBackgroundQuad();
	void CreateHeaderQuad();
	void CreateReturnQuad();

	// Get Textures
	GLuint GetLoadingTexture() const					{ return loadingTexture; }
	std::vector<GLuint> GetButtonTextures() const		{ return buttonTextures; }
	std::vector<GLuint> GetPauseButtonTextures() const	{ return pauseButtonTextures; }
	std::vector<GLuint> GetCollectibleTextures() const	{ return collectibleTextures; }
	std::vector<GLuint> GetHtpTextures() const			{ return htpTextures; }

	int GetVertexCountMainTotal() const									{ return vertexCountMainTotal; }
	std::vector<ButtonVtx> GetMainMenuButtonVertices(int idx) const		{ return mainButtons[idx].GetButtonVertices(); }
	std::vector<MenuButton> GetMainMenuButtons() const					{ return mainButtons; }
	int GetVertexCountPauseTotal() const								{ return vertexCountPauseTotal; }
	std::vector<ButtonVtx> GetPauseMenuButtonVertices(int idx) const	{ return pauseButtons[idx].GetButtonVertices(); }
	std::vector<MenuButton> GetPauseMenuButtons() const					{ return pauseButtons; }
	int GetVertexCountCollectibleTotal() const								{ return vertexCountCollectibleTotal; }
	std::vector<ButtonVtx> GetCollectibleMenuButtonVertices(int idx) const	{ return collectibleButtons[idx].GetButtonVertices(); }
	std::vector<MenuButton> GetCollectibleMenuButtons() const				{ return collectibleButtons; }
	int GetVertexCountHtpTotal() const									{ return vertexCountHtpTotal; }
	std::vector<ButtonVtx> GetHtpMenuButtonVertices(int idx) const		{ return htpButtons[idx].GetButtonVertices(); }
	std::vector<MenuButton> GetHtpMenuButtons() const					{ return htpButtons; }

	// Offset is for stacked menus, which will be used if we make a collectible menu
	float GetCurrentOffset(int nrOfButtons) const	{ return (nrOfButtons - 1) * (buttonWidth + BUTTON_OFFSET) - 0.415f; }
	int GetNrOfMenuButtons() const					{ return nrOfMainButtons; }
	int GetNrOfPauseButtons() const					{ return nrOfPauseButtons; }
	int GetNrOfCollectibleButtons() const			{ return nrOfCollectibleButtons; }
	int GetNrOfHtpButtons() const					{ return nrOfHtpButtons; }

	ButtonVtx GetBackgroundVertices(int idx) const	{ return backgroundQuad[idx]; }

	bool GetIsMenuRunning() const					{ return isMenuRunning; }
	void SetIsMenuRunning(bool tf)					{ this->isMenuRunning = tf; }

	ACTIVEMENU GetActiveMenu() const				{ return activeMenu; }
	void SetActiveMenu(ACTIVEMENU nm)				{ this->activeMenu = nm; }

	int GetLastClickedButton() const				{ return currentButtonHit; }
	void ResetLastClickedButton()					{ this->currentButtonHit = -1; }
	bool GetHasButtonActionExecuted() const			{ return buttonActionExecuted; }
	void SetButtonActionExecuted(bool tf)			{ this->buttonActionExecuted = tf; }

	GAMESTATE GetUpdateState() const				{ return updateState; }
	void ResetUpdateState()							{ updateState = MAINMENU; }

	bool GetIsLoading() const						{ return isLoading; }
	void SetIsLoading(bool isLoading)				{ this->isLoading = isLoading; }

	bool GetIsHovering() const { return isHovering; }

	void SetCollected(std::vector<Collectible> coll);
	
};

