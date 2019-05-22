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
	nrOfMainButtons = 0;
	nrOfPauseButtons = 0;
	isMenuRunning = true;
	isButtonHit = false;
	updateState = MAINMENU;

	CreateMenuTexture("Resources/Textures/PauseMenu1.png", &pauseOverlayTexture);
	CreateMenuTexture("Resources/Textures/Loading1.png", &loadingTexture);
	CreateMenuTexture("Resources/Textures/MenuButtonTEMP.png", &buttonTextureBase);
	//CreateMenuTexture("Resources/Textures/PauseButtonTEMP.png", &pauseButtonTexture);
	CreateMenuTexture("Resources/Textures/MainMenuRender.png", &backgroundTexture);

	CreateMenuTexture("Resources/Textures/PauseTitle.png", &pbt0);
	CreateMenuTexture("Resources/Textures/PauseResume.png", &pbt1);
	CreateMenuTexture("Resources/Textures/PauseRestart.png", &pbt2);
	CreateMenuTexture("Resources/Textures/PauseQuit.png", &pbt3);

	pauseButtonTextures.push_back(pbt0);
	pauseButtonTextures.push_back(pbt1);
	pauseButtonTextures.push_back(pbt2);
	pauseButtonTextures.push_back(pbt3);


	CreateMainMenu();
}

Menu::~Menu() 
{

}

// ========================================================================
//	Creates the buttons for the main menu, this also send in the offset for a "stacked" menu
// ========================================================================
void Menu::CreateMainMenu()
{
	// Creates Main Menu Background as well as Main Menu Buttons
	CreateMainMenuButtons();

	// Creates Pause Buttons (Stacked Menu)
	for (int i = 0; i < 4; i++) {
		MenuButton newPauseButton(GetCurrentOffsetPause(), i);
		vertexCountPauseTotal += newPauseButton.GetVertexCount();
		pauseButtons.push_back(newPauseButton);
		nrOfPauseButtons++;
	}

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
		CheckCollision(x, y);
		printMouseClickOnce = true;
		buttonActionExecuted = false;
	}
	else if (glfwGetMouseButton(renderWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		printMouseClickOnce = false;
	}

	if (activeMenu == MAINACTIVE) {
		if (isButtonHit == true) {
			if (currentButtonHit == 1) {
				// START GAME			// This is handled in GameEngine by getting the last clicked button
				updateState = PLAYING;
			}
			else if (currentButtonHit == 2) {
				// SETTINGS? CREDITS? HOW TO PLAY?
			}
			else if (currentButtonHit == 3) {
				// EXIT
				glfwSetWindowShouldClose(renderWindow, GL_TRUE);
			}
			else if (currentButtonHit == 4) {
				// MY TOYS / COLLECTIBLE MENU
			}
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
bool Menu::CheckCollision(float x, float y) 
{
	if (activeMenu == PAUSEACTIVE) {
		for (int i = 0; i < nrOfPauseButtons; i++) {
			if (pauseButtons[i].CheckInsideCollision(x, y) == true) {
				//std::cout << "Hit Button nr " << i << std::endl;
				currentButtonHit = i;
				return true;
			}
		}
	}
	else if (activeMenu == MAINACTIVE){
		for (int i = 0; i < nrOfMainButtons; i++) {
			if (mainButtons[i].GetIsNotButton() != true) {
				if (mainButtons[i].CheckInsideCollision(x, y) == true) {
					std::cout << "Hit Button nr " << i << std::endl;
					currentButtonHit = i;
					isButtonHit = true;
					return true;
				}
			}
		}
	}
	return false;
}

// ========================================================================
//  Creates the Main Menu Background and Main Menu Buttons
// ========================================================================
void Menu::CreateMainMenuButtons() 
{

	// Creates the background, which is not a button (and collision will not be checked on it)
	CreateBackgroundQuad();
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
	}
	
	
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
		//backgroundQuad[i] = myQuad[i];
		backgroundQuad.push_back(bgQuad[i]);
	}
}

void Menu::SetCollected(std::vector<Collectible> coll)
{
	for (int i = 0; i < COLLECTEDCAP; i++) {
		collected[i].SetCollected(coll[i].GetCollected());
	}
}
