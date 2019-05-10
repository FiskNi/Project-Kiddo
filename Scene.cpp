#include "Scene.h"


void Scene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Scene* scene = (Scene*)glfwGetWindowUserPointer(window);


	if (key == GLFW_KEY_N && action == GLFW_PRESS) 
	{
		//SWITCHES ROOM BUT ACTUALLY RESETS
		scene->SwitchRoom();
	}

	//IF PAUSED
	if (scene->state == PAUSED) {
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

		if (key == GLFW_KEY_3 && action == GLFW_PRESS) 
		{
			//CLOSES WINDOW
			//glfwSetWindowShouldClose(window, GL_TRUE);

			scene->state = MAINMENU;
			//scene->SwitchRoom();
			scene->SwitchMainMenu();
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
			//scene->CompileMeshDataPauseMenu();
			scene->state = PAUSED;
			std::cout << "PAUSED" << std::endl;
			std::cout << "Press the numbers below to perform actions: " << std::endl;
			std::cout << "1 - Resume" << std::endl;
			std::cout << "2 - Restart" << std::endl;
			std::cout << "3 - Exit" << std::endl;
		}
	}
	else if (scene->state == MAINMENU) 
	{

		if (key == GLFW_KEY_1 && action == GLFW_PRESS) 
		{
			//RESUMES GAME
			scene->state = PLAYING;
			//scene->SwitchRoom();
			scene->isSwitched = true;
			std::cout << "START GAME/RESUME" << std::endl;
			std::cout << "Loading takes time!" << std::endl;
		}

		//if (key == GLFW_KEY_2 && action == GLFW_PRESS) 
		//{
		//	//RESTART HERE
		//	scene->ResetRoom();
		//	scene->state = PLAYING;
		//	std::cout << "Restarting level" << std::endl;
		//}

		if (key == GLFW_KEY_3 && action == GLFW_PRESS) 
		{
			//CLOSES WINDOW
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}

Scene::Scene()
{

	state = MAINMENU;
	// Loads content | *Each function could return a bool incase of failure

	Loader startingRoom("Resources/Assets/GameReady/Rooms/Level1v3.meh");
	Loader secondRoom("Resources/Assets/GameReady/Rooms/Level1[Culled]Fixed.meh");
	Loader mainMenuRoom("Resources/Assets/GameReady/Rooms/Level1[Culled]Fixed.meh");

	LoadShaders();
	LoadMaterials(&startingRoom);
	LoadCharacter();
	LoadLevels();

	this->isSwitched = false;
	currentBuffer = 0;

	// Initializes startingroom. Existing materials is needed for all the entities.
	firstRoomBuffer = new Room(materials, &startingRoom);
	secondRoomBuffer = new Room(materials, &secondRoom);
	mainMenuRoomBuffer = new Room(materials, &mainMenuRoom);

	// Compiles all the meshdata of the scene for the renderer
	// CompileMeshData();
	CompileMeshDataMainMenu();
}

Scene::~Scene()
{
	delete firstRoomBuffer;
	delete secondRoomBuffer;
	delete mainMenuRoomBuffer;
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
}

void Scene::LoadMaterials(Loader* inLoader)
{
	// Initialize materials and textures
	// The constructor integer is the material id slot
	// So the first material has id #0 (materials is size 0), second has id #1, and so on

	// Hardcoded materials that will be moved
	Material planeMat("Plane Material", materials.size());
	planeMat.createAlbedo("Resources/Textures/brickwall.jpg");
	planeMat.createNormal("Resources/Textures/brickwall_normal.jpg");
	materials.push_back(planeMat);

	Material playerMat("Player Material", materials.size());
	playerMat.createAlbedo("Resources/Textures/broken.png");
	materials.push_back(playerMat);

	Material cubeMat("Plane Cube Material", materials.size());
	cubeMat.createAlbedo("Resources/Textures/boxTexture.png");
	materials.push_back(cubeMat);

	Material nodeMat("Node Material", materials.size());
	nodeMat.createAlbedo("Resources/Textures/broken.png");
	materials.push_back(nodeMat);

	for (int i = 0; i < inLoader->GetMaterialCount(); i++)
	{
		Material fillMat(inLoader->GetMaterial(i), materials.size());
		materials.push_back(fillMat);
	}

}

void Scene::LoadCharacter()
{
	// Could be improved instead of having a specific integer #, example a named integer "playerMaterial"
	playerCharacter.SetMaterialID(1);
	playerCharacter.OffsetPositionX(-3.0f);
	playerCharacter.OffsetPositionY(3.0f);
	playerCharacter.SetStartPosition(playerCharacter.GetPosition());
}

void Scene::LoadLevels()
{
}

void Scene::CompileMeshData()
{
	// Compile the mesh data of the first room
	meshes.clear();

	firstRoomBuffer->CompileMeshData();
	meshes = firstRoomBuffer->GetMeshData();
	meshes.push_back(playerCharacter.GetMeshData());
	// Compile character data
	
}

void Scene::CompileMeshDataMainMenu()
{
	// Fills the "meshes" vector with all the mesh data (primitive)
	mainMenuRoomBuffer->CompileMeshData();
	meshes.clear();

	meshes = mainMenuRoomBuffer->GetMeshData();
	//meshes.push_back(playerCharacter.GetMeshData());
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


	if (state == MAINMENU) 
	{

		CompileMeshDataMainMenu();
	}
	else if (state == PLAYING) 
	{

		Gravity();

		// Player movement vector
		glm::vec3 playerMoveVector = playerCharacter.Move(renderWindow);
		if (!playerCharacter.IsColliding())
		{
			playerCharacter.AddVelocity(playerCharacter.GetInputVector());
		}

		// First update
		playerCharacter.Update(deltaTime);

		firstRoomBuffer->Update(&playerCharacter, renderWindow, deltaTime);

		// Update the scene
		//playerCharacter.Update(deltaTime);
		for (int i = 0; i < firstRoomBuffer->GetRigids().size(); i++)
		{
			firstRoomBuffer->GetRigids()[i].Update(deltaTime);
		}

		for (int i = 0; i < firstRoomBuffer->GetBridges().size(); i++)
		{
			firstRoomBuffer->GetBridges()[i].Update(deltaTime);
		}


		// Compile render data for the renderer
		CompileMeshData();
	}
	else{
		// The PAUSED state does not update anything, it leaves movement frozen and only prints PAUSED
		// Might want to handle mouse picking here
	}
}

void Scene::SetIsSwitched(bool isSwitched)
{
	this->isSwitched = isSwitched;
}

void Scene::ResetRoom()
{
	playerCharacter.SetPosition(playerCharacter.GetRespawnPos());
	for (int i = 0; i < firstRoomBuffer->GetRigids().size(); i++)
	{
		firstRoomBuffer->GetRigids()[i].ResetPos();
	}
}

void Scene::SwitchRoom()
{
	delete firstRoomBuffer;

	firstRoomBuffer = secondRoomBuffer;

	if (roomNr == 0)
	{
		Loader temp("Resources/Assets/GameReady/Rooms/Level1v3.meh");
		secondRoomBuffer = new Room(materials, &temp);
	}
	else if (roomNr == 1)
	{
		Loader temp("Resources/Assets/GameReady/Rooms/Level1v3.meh");
		secondRoomBuffer = new Room(materials, &temp);
	}
	else
	{
		Loader temp("Resources/Assets/GameReady/Rooms/Level1v3.meh");
		secondRoomBuffer = new Room(materials, &temp);
	}
	playerCharacter.SetPosition(playerCharacter.GetRespawnPos());

	CompileMeshData();
	this->isSwitched = true;
	this->roomNr += 1;
}

void Scene::SwitchMainMenu() 
{
	if (state == MAINMENU)
	{
		CompileMeshDataMainMenu();
		this->isSwitched = true;
	}
}

//=============================================================
//	Applies basic gravity to the player and rooms
//=============================================================
void Scene::Gravity()
{
	// Our downward acceleration
	const float gravity = -2.283;

	// Entity boxes
	for (int i = 0; i < firstRoomBuffer->GetRigids().size(); i++)
	{	
		if (!firstRoomBuffer->GetRigids()[i].IsGrounded())
		{
			firstRoomBuffer->GetRigids()[i].AddVelocityY(gravity);
		}
	}

	// Player
	if (!playerCharacter.IsGrounded())
	{
		playerCharacter.AddVelocityY(gravity);
	}
}
