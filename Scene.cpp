#include "Scene.h"


void Scene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Scene* scene = (Scene*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_N && action == GLFW_PRESS && scene->state != MAINMENU)
	{
		if (scene->roomBuffer)
			scene->roomBuffer->SetRoomCompleted(true);
	}

	// IF PAUSED
	if (scene->state == PAUSED)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			//UNPAUSE
			scene->state = PLAYING;
			std::cout << "PLAYING" << std::endl;
		}

		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			//RESUMES GAME
			scene->state = PLAYING;
			std::cout << "RESUME" << std::endl;
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		{
			//RESTART HERE
			scene->ResetRoom();
			scene->state = PLAYING;
			std::cout << "Restarting level" << std::endl;
		}

		if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
			// RETURNS TO MAIN MENU
			delete scene->roomBuffer;
			scene->roomBuffer = nullptr;
			scene->roomLoaded = false;
			scene->isLoading = true;
			scene->exittoMenu = true;
			scene->state = PLAYING;

			std::cout << "Returning to Main Menu" << std::endl;
			std::cout << "Loading..." << std::endl;
			std::cout << "MAIN MENU" << std::endl;
			std::cout << "Press the numbers below to perform actions: " << std::endl;
			std::cout << "1 - Start" << std::endl;
			//std::cout << "2 - Settings" << std::endl;
			std::cout << "3 - Exit" << std::endl;
		}
	}
	// IF PLAYING
	else if (scene->state == PLAYING)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			scene->state = PAUSED;
			std::cout << "PAUSED" << std::endl;
			std::cout << "Press the numbers below to perform actions: " << std::endl;
			std::cout << "1 - Resume" << std::endl;
			std::cout << "2 - Restart" << std::endl;
			std::cout << "3 - Exit" << std::endl;
		}

		if (key == GLFW_KEY_E && action == GLFW_PRESS)
		{
			scene->_CheckPressedButtons();
			scene->_CheckPressedBombs();
		}

		// EXTRA
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
			scene->Upgrade();
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			scene->playerCharacter.SetCurrentItem(0);
		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			scene->playerCharacter.SetCurrentItem(1);
		if (key == GLFW_KEY_3 && action == GLFW_PRESS)
			scene->playerCharacter.SetCurrentItem(2);
		if (key == GLFW_KEY_4 && action == GLFW_PRESS)
			scene->playerCharacter.SetCurrentItem(3);
		if (key == GLFW_KEY_5 && action == GLFW_PRESS)
			scene->playerCharacter.SetCurrentItem(4);
	}
}

void Scene::_CheckPressedButtons()
{
	for (int i = 0; i < roomBuffer->getButtons().size(); i++)
	{
		if (!roomBuffer->getButtons()[i].isPressed() && playerCharacter.CheckCollision(roomBuffer->getButtons()[i]))
		{
			roomBuffer->getButtons()[i].SetPressed(true);
			// ADD SOUND PLAY
		}
	}
}

void Scene::_CheckPressedBombs()
{
	for (int i = 0; i < roomBuffer->GetRigids().size(); i++) 
		if (roomBuffer->GetRigids()[i].GetBoxType() == EXPLOSIVE) 
			if (playerCharacter.CheckInBound(roomBuffer->GetRigids()[i])) 
				roomBuffer->GetRigids()[i].BombTimer();
}

Scene::Scene()
{
	LoadShaders();
	roomBuffer = nullptr;
	roomNr = 0;
	isLoading = false;
	exittoMenu = false;
	roomLoaded = false;

	// If no audiodevice exists this will initiate as NULL, make sure to check that this was successful
	// when trying to play audio
	audioEngine = irrklang::createIrrKlangDevice();
	if (audioEngine)
		audioEngine->play2D("irrKlang/media/bell.wav", false);
	else
		std::cout << "Failed to create audio device, none connected?" << std::endl;

	// This is the initial state the game will be in when the update loop starts running
	state = PLAYING;
}

Scene::~Scene()
{
	if (roomBuffer)
		delete roomBuffer;

	//audioEngine->drop();
}

void Scene::LoadShaders()
{
	// Loads shaders from file
	basicShader.CreateShader("VertexShader.glsl", "Fragment.glsl");
	shaders.push_back(basicShader);

	shadowmapShader.CreateShader("VertexShaderSM.glsl", "FragmentSM.glsl");
	shaders.push_back(shadowmapShader);

	// Initialize fullscreen quad vertices
	// Right now the fullscreen quad is coded into the shader handler.
	// Could be moved and better organized
	fsqShader.CreateFSShaders();
	fsqShader.CreateFullScreenQuad();
	shaders.push_back(fsqShader);

	mainMenuShader.CreateShader("VertexShaderMenu.glsl", "FragmentMenu.glsl");
	shaders.push_back(mainMenuShader);
}

void Scene::LoadMaterials(Loader* inLoader)
{
	// Initialize materials and textures
	// The constructor integer is the material id slot
	// So the first material has id #0 (materials is size 0), second has id #1, and so on

	// Hardcoded materials that will be moved
	materials.clear();
	for (int i = 0; i < inLoader->GetMaterialCount(); i++)
	{
		Material fillMat(inLoader->GetMaterial(i), materials.size());
		materials.push_back(fillMat);
	}

}

void Scene::LoadCharacter(Loader* inLoader)
{
	// Could be improved instead of having a specific integer #, example a named integer "playerMaterial"
	
	for (int i = 0; i < inLoader->GetMeshCount(); i++)
	{
		if (inLoader->GetType(i) == 8)
		{
			playerCharacter.SetMaterialID(0);
			playerCharacter.SetPosition(inLoader->GetMesh(i).translation);
		}
	}

	playerCharacter.SetStartPosition(playerCharacter.GetPosition());
}

void Scene::CompileMeshData()
{
	// Compile the mesh data of the first room
	meshes.clear();

	roomBuffer->CompileMeshData();
	meshes = roomBuffer->GetMeshData();
	meshes.push_back(playerCharacter.GetMeshData());
	// Compile character data
}

//=============================================================
//	Everything that updates in a scene happens here. 
//	This can include character movement, world timers, world actions, gamestates etc.
//=============================================================
void Scene::Update(GLFWwindow* renderWindow, float deltaTime)
{
	if (!setUserPointer) 
	{
		glfwSetWindowUserPointer(renderWindow, this);
		glfwSetKeyCallback(renderWindow, key_callback);
		setUserPointer = true;
	}

	if (!exittoMenu && state == PLAYING)
	{ 
		// Check room completion
		// Could add extra functions here (loadingscreen or whatever)
		if (roomBuffer->GetRoomCompleted())
		{
			if (isLoading)
				roomLoaded = false;
			else
				isLoading = true;
		}
		else
		{
			Gravity();
			// Player movement vector
			playerCharacter.Move(renderWindow);
			if (!playerCharacter.IsColliding())
			{
				playerCharacter.AddVelocity(playerCharacter.GetInputVector());
			}

			// Character update
			playerCharacter.Update(deltaTime);
			// Update the scene
			for (int i = 0; i < roomBuffer->GetRigids().size(); i++)
			{
				roomBuffer->GetRigids()[i].Update(deltaTime);
			}

			for (int i = 0; i < roomBuffer->GetBridges().size(); i++)
			{
				roomBuffer->GetBridges()[i].Update(deltaTime);
			}

			roomBuffer->Update(&playerCharacter, renderWindow, deltaTime);

			// Compile render data for the renderer
			CompileMeshData();
		}	
	}
	else
	{
		// The PAUSED state does not update anything, it leaves movement frozen and only prints PAUSED
		// Might want to handle mouse picking here
	}
}

void Scene::ResetRoom()
{
	playerCharacter.ResetPos();

	// This should be a function in the room that can reset all the relevant data (bridge positions and button states)
	for (int i = 0; i < roomBuffer->GetRigids().size(); i++)
	{
		roomBuffer->GetRigids()[i].ResetPos();
	}
}

void Scene::Exited()
{
	exittoMenu = false;
}

//=============================================================
//	Loads room data
//=============================================================
void Scene::LoadRoom()
{
	if (roomBuffer)
		delete roomBuffer;
	roomBuffer = nullptr;

	Loader* roomLoader;

	// Hardcoded rooms that exists in the game. All room files are to be hardcoded here.
	// roomNr refers to the order of the levels appreance. 
	// Additional hardcoded roomfunctions may be applied here.
	if (roomNr == 0)
	{
		roomLoader = new Loader("Resources/Assets/GameReady/Rooms/Level[PadsNWalls].meh");
		// ADD SOUND PLAY
	}
	else if (roomNr == 1)
	{
		roomLoader = new Loader("Resources/Assets/GameReady/Rooms/Level[BoxConundrum].meh");
		// ADD SOUND PLAY
	}
	else if (roomNr == 2)
	{
		roomLoader = new Loader("Resources/Assets/GameReady/Rooms/Level[PadsNWalls].meh");
		// ADD SOUND PLAY
	}
	else if (roomNr == 3)
	{
		roomLoader = new Loader("Resources/Assets/GameReady/Rooms/Level[BoxConundrum].meh");
		// ADD SOUND PLAY
	}
	else
	{
		roomNr = 0;
		roomLoader = new Loader("Resources/Assets/GameReady/Rooms/Level[PadsNWalls].meh");
		// ADD SOUND PLAY
	}

	LoadMaterials(roomLoader);
	roomBuffer = new Room(materials, roomLoader, audioEngine);

	// Set player position and reset it
	for (int i = 0; i < roomLoader->GetMeshCount(); i++)
	{
		if (roomLoader->GetType(i) == 8)
		{
			playerCharacter.SetMaterialID(0);
			playerCharacter.SetPosition(roomLoader->GetMesh(i).translation);
		}
	}
	playerCharacter.SetStartPosition(playerCharacter.GetPosition());
	playerCharacter.ResetPos();

	CompileMeshData();
	roomNr++;

	isLoading = false;
	roomLoaded = true;
	delete roomLoader;
}

//=============================================================
//	Applies basic gravity to the player and rooms
//=============================================================
void Scene::Gravity()
{
	// Our downward acceleration
	const float gravity = -2.283;

	// Entity boxes
	for (int i = 0; i < roomBuffer->GetRigids().size(); i++)
	{	
		if (!roomBuffer->GetRigids()[i].IsGrounded() && !roomBuffer->GetRigids()[i].GetBoxType() == LIGHTWEIGHT)
		{
			roomBuffer->GetRigids()[i].AddVelocityY(gravity);
		}
	}

	// Player
	if (!playerCharacter.IsGrounded())
	{
		playerCharacter.AddVelocityY(gravity);
	}
}
