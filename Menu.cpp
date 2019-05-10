#include "Menu.h"


Menu::Menu() {
	pauseOverlayTexture = 0;
	CreatePauseOverlayTexture("Resources/Textures/PauseMenu1.png");
}

Menu::~Menu() {


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

void Menu::CreatePauseOverlayTexture(std::string path)
{
	// --- COPIED FROM MATERIAL CREATEALBEDO (mostly)
	//Generate Texture, 1:st argument is amt of textures second is where to store them
	glGenTextures(1, &pauseOverlayTexture);
	glBindTexture(GL_TEXTURE_2D, pauseOverlayTexture);

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
