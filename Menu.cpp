#include "Menu.h"


Menu::Menu() 
{
#pragma region initialize vector
	for (int i = 0; i < COLLECTEDCAP; i++) {
		collected.push_back(Collectible());
	}

#pragma endregion

	vertexCountMainTotal = 0;
	vertexCountPauseTotal = 0;
	vertexCountCollectibleTotal = 0;
	vertexCountHtpTotal = 0;
	nrOfMainButtons = 0;
	nrOfPauseButtons = 0;
	nrOfCollectibleButtons = 0;
	nrOfHtpButtons = 0;
	isMenuRunning = true;
	isButtonHit = false;
	updateState = MAINMENU;

	CreateMenuTexture("Resources/Textures/LoadingScreen.png", &loadingTexture);
	CreateMenuTexture("Resources/Textures/MenuButtonTEMP.png", &buttonTextureBase);
	CreateMenuTexture("Resources/Textures/PauseGUI.png", &pauseBackgroundTexture);
	CreateMenuTexture("Resources/Textures/MainMenuRender.png", &backgroundTexture);
	CreateMenuTexture("Resources/Textures/HTPtemp.png", &howToPlayTexture);

	CreateMenuTexture("Resources/Textures/PauseQuit.png", &tempCollQuit);

	CreateCollectibleTextures();	

	CreateMainMenu();
}


Menu::~Menu() 
{
	//glfwDestroyCursor(handCursor);
}

// ========================================================================
//	Creates the buttons for the main menu and pause menu
// ========================================================================
void Menu::CreateMainMenu()
{
	// Initialises all the predefined quads in -1 to 1 space
	CreateBackgroundQuad();
	CreateHeaderQuad();
	CreateReturnQuad();

	// Creates Main Menu Background as well as Main Menu Buttons
	CreateMainMenuButtons();
	// Creates Pause Menu buttons and background
	CreatePauseMenuButtons();
	// Creates Collectible Menu buttons
	CreateCollectibleMenuButtons();

	CreateHTPMenuButtons();
}

// ========================================================================
//	Updates the menu and checks for what to do whenever a menu button has been clicked
// ========================================================================
void Menu::MenuUpdate(GLFWwindow * renderWindow, float deltaTime)
{

	// ****KEY CALLBACK DOES NOT WORK FOR MENU, please do not attempt to re-implement Key callback in Menu****
	glfwPollEvents();
	if (glfwGetMouseButton(renderWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && printMouseClickOnce == false)
	{
		// Gets the clicked cursor position and checks for collision with any of the buttons
		double x, y;
		glfwGetCursorPos(renderWindow, &x, &y);
		//std::cout << "Current Cursor Position: " << x << "  " << y << std::endl;
		CheckCollision(x, y, true);
		printMouseClickOnce = true;
		buttonActionExecuted = false;
	}
	else if (glfwGetMouseButton(renderWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) 
	{
		printMouseClickOnce = false;
	}

	if (activeMenu == MAINACTIVE) 
	{
		if (isButtonHit == true) {
			if (currentButtonHit == 1) {
				// START GAME			// This is handled in GameEngine by getting the last clicked button
				updateState = PLAYING;
			}
			else if (currentButtonHit == 2) {
				// SETTINGS? CREDITS? HOW TO PLAY?
				activeMenu = HTPACTIVE;
			}
			else if (currentButtonHit == 3) {
				// EXIT
				glfwSetWindowShouldClose(renderWindow, GL_TRUE);
			}
			else if (currentButtonHit == 4) {
				// MY TOYS / COLLECTIBLE MENU
				activeMenu = COLLECTIBLEACTIVE;
			}
			currentButtonHit = -1;
			isButtonHit = false;
		}
	}
	else if (activeMenu == COLLECTIBLEACTIVE)
	{
		if (isButtonHit == true) {
			if (currentButtonHit == 1) {
				// START GAME			// This is handled in GameEngine by getting the last clicked button
				//activeMenu = MAINACTIVE;
			}
			else if (currentButtonHit == 9) {
				// EXIT
				//glfwSetWindowShouldClose(renderWindow, GL_TRUE);
				activeMenu = MAINACTIVE;
			}
			currentButtonHit = -1;
			isButtonHit = false;
		}
	}
	else if (activeMenu == HTPACTIVE) {
		if (isButtonHit == true) {
			if (currentButtonHit == 0) {
				activeMenu = MAINACTIVE;
			}
			currentButtonHit = -1;
			isButtonHit = false;
		}
	}
	else if (activeMenu == PAUSEACTIVE) {
		if (isButtonHit == true) {
			currentButtonHit = -1;
			isButtonHit = false;
		}
	}
}

// =============================================================
//	Creates a texture for the menu based on texture file path and which GLuint to put the texture data to
// =============================================================
void Menu::CreateMenuTexture(std::string path, GLuint *texture)
{
	// --- COPIED FROM MATERIAL CREATEALBEDO (mostly)
	//Generate Texture, 1:st argument is amt of textures second is where to store them
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width,
		height,
		colourChannels;

	const char* filePath = path.c_str();
	// Flips the imported texture according to OpenGL UVs
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &width, &height, &colourChannels, 0);
	if (data)
	{	// Should be transparent now, but it does not render transparent, might be a problem here or with the shaders
		//Function arguments:  | Target | Mipmap | Image format | Width | Height | Legacy, need to be 0 | Format | Data type | Image data |
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture. Reason: " << stbi_failure_reason() << std::endl;
	stbi_image_free(data);
}

// ========================================================================
//	Checks collision between the clicked mouse cursor and the buttons
// ========================================================================
bool Menu::CheckCollision(float x, float y, bool isClicked) 
{
	if (activeMenu == PAUSEACTIVE) 
	{
		for (int i = 0; i < nrOfPauseButtons; i++) 
		{
			if (mainButtons[i].GetIsNotButton() != true) 
			{
				if (pauseButtons[i].CheckInsideCollision(x, y) == true)
				{
					if (isClicked == true) {
						//std::cout << "Hit Button nr " << i << std::endl;
						currentButtonHit = i;
					}
					return true;
				}
			}
		}
	}
	else if (activeMenu == MAINACTIVE)
	{
		for (int i = 0; i < nrOfMainButtons; i++) 
		{
			if (mainButtons[i].GetIsNotButton() != true) 
			{
				if (mainButtons[i].CheckInsideCollision(x, y) == true) 
				{
					if (isClicked == true) {
						//std::cout << "Hit Button nr " << i << std::endl;
						currentButtonHit = i;
						isButtonHit = true;
					}
					return true;
				}
			}
		}
	}
	else if (activeMenu == COLLECTIBLEACTIVE)
	{
		for (int i = 0; i < nrOfCollectibleButtons; i++)
		{
			if (collectibleButtons[i].GetIsNotButton() != true)
			{
				if (collectibleButtons[i].CheckInsideCollision(x, y) == true)
				{
					if (isClicked == true) {
						std::cout << "Hit Button nr " << i << std::endl;
						currentButtonHit = i;
						isButtonHit = true;
					}
					return true;
				}
			}
		}
	}
	else if (activeMenu == HTPACTIVE)
	{
		for (int i = 0; i < nrOfHtpButtons; i++)
		{
			if (htpButtons[i].GetIsNotButton() != true)
			{
				if (htpButtons[i].CheckInsideCollision(x, y) == true)
				{
					if (isClicked == true) {
						//std::cout << "Hit Button nr " << i << std::endl;
						currentButtonHit = i;
						isButtonHit = true;
					}
					return true;
				}
			}
		}
	}
	return false;
}

bool Menu::CheckButtonHovering(GLFWwindow * renderWindow) {
	double x, y;
	glfwGetCursorPos(renderWindow, &x, &y);

	if (CheckCollision(x, y, false)) {
		return true;
	}
	else {
		return false;
	}
}

// ========================================================================
//  Creates the Main Menu Background and Main Menu Buttons
// ========================================================================
void Menu::CreateMainMenuButtons() 
{
	// Creates the background, which is not a button (and collision will not be checked on it)
	//CreateBackgroundQuad();
	buttonTextures.push_back(backgroundTexture);
	MenuButton bgButton(backgroundQuad, 0, true);
	mainButtons.push_back(bgButton);


	//buttonTextures.push_back(buttonTextureBase);
	// schh... it works ;^)
	buttonTextures.push_back(backgroundTexture);

	// Start Button
	MenuButton newButton(1290, 105, 1610, 220, 1);
	mainButtons.push_back(newButton);

	// Settings Button
	MenuButton newButton1(1420, 280, 1730, 480, 1);
	mainButtons.push_back(newButton1);

	// Exit Button
	MenuButton newButton2(1650, 750, 1840, 900, 1);
	mainButtons.push_back(newButton2);

	// My Toys Button (Collectibles)
	MenuButton newButton3(830, 630, 1220, 850, 1);
	mainButtons.push_back(newButton3);

	for (int i = 0; i < 5; i++) {
		vertexCountMainTotal += newButton.GetVertexCount();		// Vertex Count is ALWAYS 6 for all buttons
		nrOfMainButtons++;
		//buttonTextures.push_back(backgroundTexture);
	}
	
}

// ========================================================================
//  Creates the Pause Menu Background and Pause Menu Buttons
// ========================================================================
void Menu::CreatePauseMenuButtons()
{
	// Creates the background, which is not a button (and collision will not be checked on it)
	// backgroundQuad has already been initialised in CreateMainMenuButtons()
	pauseButtonTextures.push_back(pauseBackgroundTexture);
	MenuButton bgButton(backgroundQuad, 0, true);
	pauseButtons.push_back(bgButton);


	pauseButtonTextures.push_back(pauseBackgroundTexture);

	// Start Button
	MenuButton newButton(750, 380, 1100, 495, 1);
	pauseButtons.push_back(newButton);

	// Settings Button
	MenuButton newButton1(715, 500, 1100, 600, 1);
	pauseButtons.push_back(newButton1);

	// Exit Button
	MenuButton newButton2(800, 605, 1000, 690, 1);
	pauseButtons.push_back(newButton2);

	for (int i = 0; i < 4; i++) {
		vertexCountPauseTotal += bgButton.GetVertexCount();		// Vertex count for buttons is always 6
		nrOfPauseButtons++;
	}

}

// ========================================================================
//  Creates the Collectible Menu Buttons
// ========================================================================
void Menu::CreateCollectibleMenuButtons()
{

	collectibleTextures.push_back(pauseBackgroundTexture);
	MenuButton hdButton(headerQuad, 0, true);
	collectibleButtons.push_back(hdButton);

	//collectibleTextures.push_back(buttonTextureBase);
	for (int i = 1; i < COLLECTEDCAP+1; i++) {
		//collectibleTextures.push_back(buttonTextureBase);
		MenuButton colButton(GetCurrentOffset(nrOfCollectibleButtons), i);
		collectibleButtons.push_back(colButton);
		vertexCountCollectibleTotal += colButton.GetVertexCount();		// Vertex count for buttons is always 6
		nrOfCollectibleButtons++;
	}

	collectibleTextures.push_back(pauseBackgroundTexture);
	MenuButton rtButton(returnQuad, COLLECTEDCAP+1, false);
	collectibleButtons.push_back(rtButton);

	vertexCountCollectibleTotal += hdButton.GetVertexCount();		// Vertex count for buttons is always 6
	nrOfCollectibleButtons++;
	vertexCountCollectibleTotal += hdButton.GetVertexCount();		// Vertex count for buttons is always 6
	nrOfCollectibleButtons++;

	//collectibleTextures.push_back(backgroundTexture);
	//MenuButton bgButton(backgroundQuad, COLLECTEDCAP, true);
	//collectibleButtons.push_back(bgButton); 
	//vertexCountCollectibleTotal += bgButton.GetVertexCount();		// Vertex count for buttons is always 6
	//nrOfCollectibleButtons++;


	// add this button last, this is never rendered, but this holds the first button's texture
	MenuButton newButton2(1921, 1081, 1941, 1101, COLLECTEDCAP+2);
	collectibleButtons.push_back(newButton2);

}


// ========================================================================
//  Creates the Pause Menu Background and Pause Menu Buttons
// ========================================================================
void Menu::CreateHTPMenuButtons()
{
	// Creates the background, which is not a button (and collision will not be checked on it)
	// backgroundQuad has already been initialised in CreateMainMenuButtons()
	htpTextures.push_back(howToPlayTexture);
	MenuButton bgButton(backgroundQuad, 0, false);
	htpButtons.push_back(bgButton);

	//// Start Button
	//MenuButton newButton(750, 380, 1100, 495, 1);
	//pauseButtons.push_back(newButton);

	vertexCountHtpTotal += bgButton.GetVertexCount();		// Vertex count for buttons is always 6
	nrOfHtpButtons++;


}

void Menu::CreateCollectibleTextures()
{
	// TESTING COLLECTIBLE TEXTURES
	// apparently the first button's texture is always te same as the last texture
	//collectibleTextures.push_back(buttonTextureBase);
	collectibleTextures.push_back(backgroundTexture);			// dummy texture, will not be used at the end because of a bug
	collectibleTextures.push_back(pauseBackgroundTexture);		// Collectible 1
	collectibleTextures.push_back(backgroundTexture);			// Collectible 2
	collectibleTextures.push_back(loadingTexture);				// Collectible 3
	collectibleTextures.push_back(pauseBackgroundTexture);		// Collectible 4
	collectibleTextures.push_back(backgroundTexture);			// Collectible 5
	collectibleTextures.push_back(loadingTexture);				// Collectible 6
	collectibleTextures.push_back(pauseBackgroundTexture);		// Collectible 7
	collectibleTextures.push_back(backgroundTexture);			// Collectible 8
	collectibleTextures.push_back(tempCollQuit);				// Return Button
	collectibleTextures.push_back(buttonTextureBase);			// Header Button,	This last texture is the header texture due to a bug

	//collectibleTextures.push_back(loadingTexture);
	//collectibleTextures.push_back(pauseBackgroundTexture);
}

// ========================================================================
//  Creates the "Fullscreen quad" for the background
// ========================================================================
void Menu::CreateBackgroundQuad() {

	ButtonVtx bgQuad[6] =
	{
		-1, -1,	0,		0, 0,	// TOP		LEFT
		-1, +1,	0,		0, 1,	// BOTTOM	LEFT
		+1, +1,	0,		1, 1,	// BOTTOM	RIGHT
		-1, -1,	0,		0, 0,	// TOP		LEFT
		+1, +1,	0,		1, 1,	// BOTTOM	RIGHT
		+1, -1,	0,		1, 0,	// TOP		RIGHT
	};

	for (int i = 0; i < 6; i++) {
		backgroundQuad.push_back(bgQuad[i]);
	}
}

void Menu::CreateHeaderQuad() {

	ButtonVtx bgQuad[6] =
	{
		-0.5, +0.3,	0,		0, 0,	// TOP		LEFT
		-0.5, +0.7,	0,		0, 1,	// BOTTOM	LEFT
		+0.5, +0.7,	0,		1, 1,	// BOTTOM	RIGHT
		-0.5, +0.3,	0,		0, 0,	// TOP		LEFT
		+0.5, +0.7,	0,		1, 1,	// BOTTOM	RIGHT
		+0.5, +0.3,	0,		1, 0,	// TOP		RIGHT
	};

	for (int i = 0; i < 6; i++) {
		headerQuad.push_back(bgQuad[i]);
	}
}

void Menu::CreateReturnQuad() {

	ButtonVtx bgQuad[6] =
	{
		+0.4, -0.8,	0,		0, 0,	// TOP		LEFT
		+0.4, -0.5,	0,		0, 1,	// BOTTOM	LEFT
		+0.8, -0.5,	0,		1, 1,	// BOTTOM	RIGHT
		+0.4, -0.8,	0,		0, 0,	// TOP		LEFT
		+0.8, -0.5,	0,		1, 1,	// BOTTOM	RIGHT
		+0.8, -0.8,	0,		1, 0,	// TOP		RIGHT
	};

	for (int i = 0; i < 6; i++) {
		returnQuad.push_back(bgQuad[i]);
	}
}

void Menu::SetCollected(std::vector<Collectible> coll)
{
	for (int i = 0; i < COLLECTEDCAP; i++) {
		collected[i].SetCollected(coll[i].GetCollected());
	}
}
