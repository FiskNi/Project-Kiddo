#include "Scene.h"


//void Scene::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
//{
//	Scene* scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//		scene->press();
//		scene->PauseMenu();
//
//	}
//}

Scene::Scene()
{

	state = 1;
	// Loads content | *Each function could return a bool incase of failure

	Loader startingRoom("Resources/Assets/GameReady/Rooms/TestFile.meh");
	LoadShaders();
	LoadMaterials(&startingRoom);
	LoadCharacter();
	LoadLevels();


	currentBuffer = 0;

	// Initializes startingroom. Existing materials is needed for all the entities.
	firstRoomBuffer = new Room(materials, &startingRoom);
	secondRoomBuffer = new Room(materials, &startingRoom);

	// Compiles all the meshdata of the scene for the renderer
	CompileMeshData();
}

Scene::~Scene()
{

	delete firstRoomBuffer;
	delete secondRoomBuffer;
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

	Material cubeMat("Plane Cube Material", materials.size());
	cubeMat.createAlbedo("Resources/Textures/boxTexture.png");
	materials.push_back(cubeMat);

	Material playerMat("Player Material", materials.size());
	playerMat.createAlbedo("Resources/Textures/broken.png");
	materials.push_back(playerMat);

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

	// Compile character data
	meshes.push_back(playerCharacter.GetMeshData());
}

//=============================================================
//	Everything that updates in a scene happens here. 
//	This can include character movement, world timers, world actions, gamestates etc.
//=============================================================
void Scene::Update(GLFWwindow* renderWindow, float deltaTime)
{
	//glfwSetKeyCallback(renderWindow, key_callback);

	// Checks if ESC is pressed to switch the state between PLAYING and PAUSED
	if (glfwGetKey(renderWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		keyPress = true;
		if (callOnce != true) 
		{
			if (state == PAUSE) 
				state = PLAYING;		
			else 
				state = PAUSE;

			// Sets printOnce to false so the states will print PLAYING or PAUSED depending on active state
			if (printOnce)
				printOnce = false;

			// Sets callOnce to true so this function won't keep switching between PLAYING and PAUSE as ESC is held down
			//		If this isn't used, the accuracy of tapping ESC will be unclear, and it might not switch states correctly.
			callOnce = true;
		}
	}
	if (keyPress && glfwGetKey(renderWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE) 
	{
		keyPress = false;
		// Reset callOnce to false once the key has been let go of
		callOnce = false;
	}

	if (glfwGetKey(renderWindow, GLFW_KEY_N) == GLFW_PRESS)
	{
		keyPress = true;
		SwitchRoom();

	}

	if (keyPress && glfwGetKey(renderWindow, GLFW_KEY_N) == GLFW_RELEASE) {
		keyPress = false;
	}

	if (state == PLAYING)
	{
		if (printOnce != true) 
		{
			std::cout << "PLAYING" << std::endl;
			printOnce = true;
		}

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


	if (state == PAUSE) 
	{
		// The PAUSED state does not update anything, it leaves movement frozen and only prints PAUSED
		if (printOnce != true) 
		{
			std::cout << "PAUSED" << std::endl;
			printOnce = true;
		}
	}
}

void Scene::SwitchRoom()
{
	/*delete rooms[0];

	rooms[0] = rooms[1];
	if (roomNr == 0)
	{
		Loader temp("Resources/Assets/GameReady/Rooms/Level1[Culled]Fixed.meh");
		rooms[1] = new Room(materials, temp);
	}
	else if (roomNr == 1)
	{
		Loader temp("Resources/Assets/GameReady/Rooms/Level1[Culled]Fixed.meh");
		rooms[1] = new Room(materials, temp);
	}
	else
	{
		Loader temp("Resources/Assets/GameReady/Rooms/Level1[Culled]Fixed.meh");
		rooms[1] = new Room(materials, temp);
	}
	playerCharacter.SetPosition(playerCharacter.GetRespawnPos());

	this->roomNr += 1;*/
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