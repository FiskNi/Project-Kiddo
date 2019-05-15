#include "Menu.h"


Menu::Menu() 
{
	//pauseOverlayTexture = 0;
	vertexCountTotal = 0;
	nrOfMenuButtons = 0;
	isMenuRunning = true;
	CreateMenuTexture("Resources/Textures/PauseMenu1.png", &pauseOverlayTexture);
	CreateMenuTexture("Resources/Textures/Loading1.png", &loadingTexture);
	CreateMenuTexture("Resources/Textures/MenuButtonTEMP.png", &buttonTextureBase);

	CreateMainMenu();
}

Menu::~Menu() 
{

}

//void Menu::CompileMainMenuMeshData() {
//	//// Defines const states here, check to see which room to load
//	//const int PAUSE = 2;
//
//	//// Check which state is active, and run loading accordingly
//	//if (state == PAUSE)
//	//{
//	//	// Hardcoded quad to print something to the screen
//	//	RigidEntity quad(0);
//	//	quad.SetPosition(glm::vec3(-8.0f, 5.0f, 3.0f));
//	//	quad.SetMaterialID(materials[0].getMaterialID());
//	//	quad.SetStartPosition(glm::vec3(-8.0f, 5.0f, 3.0f));
//	//	rigids.push_back(quad);
//
//	//	// Perhaps change position for the menu?
//	//	// Initialize camera (Default constructor)
//	//	roomCamera = new Camera;
//	//}
//
//	//// Compiles all the mesh data in the room for the renderer
//	//CompileMeshData();
//}

void Menu::CreateMainMenu()
{
	for (int i = 0; i < 3; i++) {
		MenuButton newButton(GetCurrentOffset(), 0);
		vertexCountTotal += newButton.GetVertexCount();
		menuButtons.push_back(newButton);
		nrOfMenuButtons++;
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


//=============================================================
//	MENU BUTTON QUADS ARE NOW CREATED IN MENU BUTTON
//=============================================================
//void Menu::CreateMenuQuad()
//{
//	// Height of one button
//	float buttonHeight = 0.5f;
//	// calculates the offset for this menu button
//	float ofs = nrOfMenuButtons * (buttonHeight + BUTTON_OFFSET);
//	// Add an offset to the y coordinate in order to offset upcoming buttons once the shape has been defined
//	MenuButton mButton = {
//		-0.5,(-0.5 + ofs),0.0,	0.0, 0.0,	//0.5, 0.5, 0.5,	// TOP		LEFT
//		-0.5,(+0.5 + ofs),0.0,	0.0, 1.0,	//0.5, 0.5, 0.5,	// BOTTOM	LEFT
//		+0.5,(+0.5 + ofs),0.0,	1.0, 1.0,	//0.5, 0.5, 0.5,	// BOTTOM	RIGHT
//		-0.5,(-0.5 + ofs),0.0,	0.0, 0.0,	//0.5, 0.5, 0.5,	// TOP		LEFT
//		+0.5,(+0.5 + ofs),0.0,	1.0, 1.0,	//0.5, 0.5, 0.5,	// BOTTOM	RIGHT
//		+0.5,(-0.5 + ofs),0.0,	1.0, 0.0,	//0.5, 0.5, 0.5,	// TOP		RIGHT
//	};
//
//	menuButtons.push_back(mButton);
//
//	//vertexCount = (int)menuButtons.size();
//
//	nrOfMenuButtons++;
//
//};

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