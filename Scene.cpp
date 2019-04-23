#include "Scene.h"



Scene::Scene()
{
	// Loads content | *Each function could return a bool incase of failure
	LoadShaders();
	LoadMaterials();
	LoadCharacter();

	// Initializes startingroom. Existing materials is needed for all the entities.
	startingRoom = new Room(materials);

	// Compiles all the meshdata of the scene for the renderer
	CompileMeshData();
}

Scene::~Scene()
{
	delete startingRoom;
}

std::vector<Light> Scene::GetPointLights() const
{
	return startingRoom->GetPointLights();
}

std::vector<DirectionalLight> Scene::GetDirectionalLights() const
{
	return startingRoom->GetDirectionalLights();
}

std::vector<Material> Scene::GetMaterials() const
{
	return materials;
}

Shader Scene::GetShader(unsigned int i) const
{
	return shaders[i];
}

std::vector<Primitive> Scene::GetMeshData() const
{
	return meshes;
}

Camera Scene::GetCamera() const
{
	return *(startingRoom->GetCamera());
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

void Scene::LoadMaterials()
{
	// Initialize materials and textures
	// The constructor integer is the material id slot
	// So the first material has id #0 (materials is size 0), second has id #1, and so on
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
}

void Scene::LoadCharacter()
{
	// Could be improved instead of having a specific integer #, example a named integer "playerMaterial"
	playerCharacter.setMaterialID(2);
}

void Scene::CompileMeshData()
{
	// Fills the "meshes" vector with all the mesh data (primitive)
	startingRoom->CompileMeshData();
	meshes.clear();
	meshes = startingRoom->GetMeshData();
	meshes.push_back(playerCharacter.getMeshData());
}

//=============================================================
//	Scene updates
//	Everything that updates in a scene happens here. 
//	This can includes collisions, camera movement,
//	character movement, world timers, world actions, etc.
//=============================================================
void Scene::Update(GLFWwindow* renderWindow, float deltaTime)
{
	startingRoom->GetCamera()->FPSCamControls(renderWindow, deltaTime);

	// Character Handling
	bool collision = false;

	// Check a potential new position
	glm::vec3 newPos = playerCharacter.Move(renderWindow, deltaTime);

	int dominatingBox = -1;

	// Collision functions
	PlayerBoxCollision(collision, newPos, dominatingBox);
	BoxBoxCollision(dominatingBox);
	BoxNodeCollision();

	if (!collision)
	{
		playerCharacter.setPosition(newPos);
		collision = false;
	}

	CompileMeshData();
}

//=============================================================
//	Scene updates
//	Checks collision from the player to each box in the scene
//	Currently it only handles the entites in the starting room
//	This will be changed as more rooms are added
//=============================================================
void Scene::PlayerBoxCollision(bool& collision, glm::vec3 &newPos, int& dominatingBox)
{
	for (int i = 0; i < startingRoom->GetEntities().size(); ++i)
	{
		if (playerCharacter.CheckCollision(startingRoom->GetEntities()[i]))
		{
			collision = true;
			// Reset player position (new position is inside a collision this the character has to be moved back again)
			glm::vec3 pushDir = startingRoom->GetEntities()[i].getPosition() - newPos;
			if (abs(pushDir.x) >= abs(pushDir.z))
				pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
			else
				pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);

			pushDir = glm::normalize(pushDir);
			pushDir *= 0.15f;

			startingRoom->MoveEntity(i, startingRoom->GetEntities()[i].getPosition() + pushDir);

			dominatingBox = i;
		}
	}
}

//=============================================================
//	Scene updates
//	Checks collision from the a box to each other box in the scene
//	Currently it only handles the entites in the starting room
//	This will be changed as more rooms are added
//=============================================================
void Scene::BoxBoxCollision(int dominatingBox)
{
	// Could possibly be done with recursion to check subsequent collisions
	// Could be made better with proper physic calculations
	for (int i = 0; i < startingRoom->GetEntities().size(); ++i)
	{
		for (int j = 0; j < startingRoom->GetEntities().size(); ++j)
		{
			if (i != j && startingRoom->GetEntities()[i].CheckCollision(startingRoom->GetEntities()[j]) && j != dominatingBox)
			{
				glm::vec3 pushDir = startingRoom->GetEntities()[j].getPosition() - startingRoom->GetEntities()[i].getPosition();
				if (abs(pushDir.x) >= abs(pushDir.z))
					pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
				else
					pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
				pushDir = glm::normalize(pushDir);
				pushDir *= 0.15f;
				startingRoom->MoveEntity(j, startingRoom->GetEntities()[j].getPosition() + pushDir);
			}
		}
	}
}

//=============================================================
//	Scene updates
//	Checks collision from a node to each other box in the scene
//	Currently it only handles the entites in the starting room
//	This will be changed as more rooms are added
//=============================================================
void Scene::BoxNodeCollision()
{
	for (int i = 0; i < startingRoom->GetEntities().size(); i++)
	{
		if (startingRoom->GetNodes()[0].CheckCollision(startingRoom->GetEntities()[i]))
		{
			cout << "Solved" << endl;
		}
	}
}



