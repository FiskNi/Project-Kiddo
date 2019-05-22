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

	CreateMenuTexture("Resources/Textures/PauseTitle.png", &pbt0);
	CreateMenuTexture("Resources/Textures/PauseResume.png", &pbt1);
	CreateMenuTexture("Resources/Textures/PauseRestart.png", &pbt2);
	CreateMenuTexture("Resources/Textures/PauseQuit.png", &pbt3);

	pauseButtonTextures.push_back(pbt0);
	pauseButtonTextures.push_back(pbt1);
	pauseButtonTextures.push_back(pbt2);
	pauseButtonTextures.push_back(pbt3);

	//CreateMenuTexture("Resources/Textures/MainMenuRender.png", &backgroundTexture);

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
	//pauseButtonTextures.push_back(pauseButtonTexture);

	for (int i = 0; i < 3; i++) 
	{
		buttonTextures.push_back(buttonTextureBase);
		MenuButton newButton(GetCurrentOffset(), i);
		vertexCountMainTotal += newButton.GetVertexCount();
		mainButtons.push_back(newButton);
		nrOfMainButtons++;
	}

	for (int i = 0; i < 4; i++) 
	{
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
	else if (glfwGetMouseButton(renderWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) 
	{
		printMouseClickOnce = false;
	}

	if (activeMenu == MAINACTIVE) 
	{
		if (isButtonHit == true) {
			if (currentButtonHit == 0) 
			{
				// START GAME			// This is handled in GameEngine by getting the last clicked button
				updateState = PLAYING;
			}
			else if (currentButtonHit == 1) 
			{
				// SETTINGS? CREDITS? HOW TO PLAY?
			}
			else if (currentButtonHit == 2) 
			{
				// EXIT
				glfwSetWindowShouldClose(renderWindow, GL_TRUE);
			}
			isButtonHit = false;
		}
	}
	//else if (activeMenu == PAUSEACTIVE) {
	//	if (isButtonHit == true) {
	//		//if (currentButtonHit == 0) {
	//		//	// DO NOTHING HERE, TOP PAUSE BUTTON SHOULD JUST BE A TEXTURE SAYING PAUSE
	//		//	// This shuts the entire window currently as a backup for testing
	//		//	glfwSetWindowShouldClose(renderWindow, GL_TRUE);
	//		//}
	//		//else if (currentButtonHit == 1) {
	//		//	// RESUME GAME			// CHANGE STATE IN SCENE, let gameengine know it needs to change state
	//		//	std::cout << "CLICKED BUTTON " << currentButtonHit << std::endl;
	//		//	//updateState = PLAYING;
	//		//}
	//		//else if (currentButtonHit == 2) {
	//		//	// RESTART
	//		//	//updateState = PLAYING;
	//		//}
	//		//else if (currentButtonHit == 3) {
	//		//	// EXIT TO MAIN MENU
	//		//	updateState = MAINMENU;
	//		//}
	//		isButtonHit = false;
	//	}
	//}
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
	if (activeMenu == PAUSEACTIVE) 
	{
		for (int i = 0; i < nrOfPauseButtons; i++) 
		{
			if (pauseButtons[i].CheckInsideCollision(x, y) == true) 
			{
				//std::cout << "Hit Button nr " << i << std::endl;
				currentButtonHit = i;
				//isButtonHit = true;
				return true;
			}
		}
	}
	else if (activeMenu == MAINACTIVE)
	{
		for (int i = 0; i < nrOfMainButtons; i++) 
		{
			if (mainButtons[i].CheckInsideCollision(x, y) == true) 
			{
				std::cout << "Hit Button nr " << i << std::endl;
				currentButtonHit = i;
				isButtonHit = true;
				return true;
			}
		}
	}
	return false;
}

void Menu::CreateBackgroundQuad() 
{

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
		collected[i].SetIndex(coll[i].GetIndex());
	}
}


//void Menu::CreateMainMenuRoom(std::vector<Material> materials, Loader* aLoader, int state)
//{
//	// This is to initialise the Main Menu room scene, specifically to place the camera in a specific spot
//
//	// Check which state is active, and run loading accordingly
//	if (state == MAINMENU)
//	{
//		// Hardcoded quad to print something to the screen
//		//LoadLights();
//		LoadEntities(materials, aLoader);
//
//		// Perhaps change position for the menu?
//		// Initialize camera (Default constructor)
//		menuCamera = new Camera;
//	}
//
//	// Compiles all the mesh data in the room for the renderer
//	CompileMeshData();
//}
//
//
////=============================================================
////	Compiles mesh data for the renderer
////=============================================================
//void Menu::CompileMeshData()
//{
//	// NEEDS TO BE CHANGED SO THE VECTOR DOESNT REALLOCATED ALL THE TIME
//	meshes.clear();
//
//	for (int i = 0; i < menuMeshes.size(); i++)
//	{
//		meshes.push_back(menuMeshes[i]);
//	}
//
//}
//
////=============================================================
////	Entity initialization
////	Loads and positions all the entities in the scene
////=============================================================
//void Menu::LoadEntities(std::vector<Material> materials, Loader* level)
//{
//
//	//==========
//	// Entity loading will be changed to take in custom attributes and base what is loaded into the room on these
//	// Will need to move the Loader up from this location to properly take in materials as well
//	// The pipeline needs to be looked over in general to determine how things will load and be created
//	//==========
//
//	for (int i = 0; i < level->GetMeshCount(); i++)
//	{
//		//Custom attributes to be detected here before pushed into the appropriate category?
//		switch (level->GetType(i))
//		{
//		case 0:		// Mesh
//		{
//			Mesh mesh(level->GetVerticies(i), level->GetVertexCount(i), materials[0].GetMaterialID());
//			menuMeshes.push_back(mesh);
//		}
//		break;
//		case 14:	// Menu Button
//
//			break;
//		default:
//			break;
//		}
//
//
//
//	}
//}