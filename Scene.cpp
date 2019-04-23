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

	ApplyGravity(deltaTime);

	// Check a potential new position
	glm::vec3 newPos = playerCharacter.Move(renderWindow, deltaTime);

	int dominatingBox = -1;

	// Collision functions
	PlayerBoxCollision(collision, newPos, dominatingBox);
	BoxBoxCollision(dominatingBox);
	BoxNodeCollision();
	RigidStaticCollision();
	RigidGroundCollision();

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
	for (int i = 0; i < startingRoom->GetRigids().size(); ++i)
	{
		if (playerCharacter.CheckCollision(startingRoom->GetRigids()[i]))
		{
			collision = true;

			if (!startingRoom->GetRigids()[i].isColliding())
			{
				// Reset player position (new position is inside a collision this the character has to be moved back again)
				glm::vec3 pushDir = startingRoom->GetRigids()[i].GetPosition() - newPos;
				if (abs(pushDir.x) >= abs(pushDir.z))
					pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
				else
					pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);

				pushDir = glm::normalize(pushDir);
				pushDir *= 0.15f;

				startingRoom->GetRigids()[i].SetPosition(startingRoom->GetRigids()[i].GetPosition() + pushDir);

				// Move lights with the entites (temporary)
				glm::vec3 newLightPos = glm::vec3(startingRoom->GetRigids()[i].GetPosition().x, 1.0f, startingRoom->GetRigids()[i].GetPosition().z);
				startingRoom->GetPointLights()[i].setLightPos(newLightPos);

				dominatingBox = i;
			}
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
	for (int i = 0; i < startingRoom->GetRigids().size(); ++i)
	{
		for (int j = 0; j < startingRoom->GetRigids().size(); ++j)
		{
			if (i != j && startingRoom->GetRigids()[i].CheckCollision(startingRoom->GetRigids()[j]) && j != dominatingBox)
			{
				if (!startingRoom->GetRigids()[j].isColliding())
				{
					glm::vec3 pushDir = startingRoom->GetRigids()[j].GetPosition() - startingRoom->GetRigids()[i].GetPosition();
					if (abs(pushDir.x) >= abs(pushDir.z))
						pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
					else
						pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
					pushDir = glm::normalize(pushDir);
					pushDir *= 0.15f;
					startingRoom->GetRigids()[j].SetPosition(startingRoom->GetRigids()[j].GetPosition() + pushDir);
				}
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
	for (int i = 0; i < startingRoom->GetRigids().size(); i++)
	{
		if (startingRoom->GetNodes()[0].CheckCollision(startingRoom->GetRigids()[i]))
		{
			for (int j = 0; j < startingRoom->GetRigids().size(); ++j)
			{
				cout << "Solved" << endl;
			}
		}
	}
}

void Scene::RigidStaticCollision()
{
	for (int i = 0; i < startingRoom->GetRigids().size(); i++)
	{
		for (int j = 0; j < startingRoom->GetStatics().size(); ++j)
		{
			if (startingRoom->GetRigids()[i].CheckCollision(startingRoom->GetStatics()[j]))
			{
				
			}
			else
			{
				
			}
		}
	}
}


void Scene::RigidGroundCollision()
{
	for (int i = 0; i < startingRoom->GetRigids().size(); i++)
	{
		for (int j = 0; j < startingRoom->GetStatics().size(); ++j)
		{
			if (startingRoom->GetRigids()[i].CheckCollision(startingRoom->GetStatics()[j]))
			{
				startingRoom->GetRigids()[i].setGrounded(true);
				startingRoom->GetRigids()[i].setSpeed(0.0f);
			}
			else
			{
				startingRoom->GetRigids()[i].setGrounded(false);
				if (startingRoom->GetRigids()[i].GetPosition().y < startingRoom->GetStatics()[j].GetBottom())
				{
					startingRoom->GetRigids()[i].SetPosition(glm::vec3(startingRoom->GetRigids()[i].GetPosition().x,
																	   startingRoom->GetRigids()[i].GetPosition().y + 0.001f,
																	   startingRoom->GetRigids()[i].GetPosition().z));
				}
			}
		}
	}
}

void Scene::ApplyGravity(float deltaTime)
{
	float gravityConst = 0.8f;
	float acceleration = 0.0f;

	for (int i = 0; i < startingRoom->GetRigids().size(); i++)
	{	
		if (!startingRoom->GetRigids()[i].isGrounded())
		{
			float ePosX = startingRoom->GetRigids()[i].GetPosition().x;
			float ePosY = startingRoom->GetRigids()[i].GetPosition().y;
			float ePosZ = startingRoom->GetRigids()[i].GetPosition().z;

			float speed = startingRoom->GetRigids()[i].getSpeed();

			speed += gravityConst * deltaTime;

			startingRoom->GetRigids()[i].setSpeed(speed);
			ePosY -= speed;

			startingRoom->GetRigids()[i].SetPosition(glm::vec3(ePosX, ePosY, ePosZ));
		}

	}

	cout << startingRoom->GetRigids()[0].getSpeed() << endl;

}



