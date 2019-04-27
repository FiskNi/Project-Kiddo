#include "Room.h"



Room::Room(std::vector<Material> materials)
{
	LoadLights();
	LoadEntities(materials);
	LoadPuzzleNode(materials);


	// Initialize camera (Default constructor)
	roomCamera = new Camera;

	// Compile all the mesh data in the room for the renderer
	// This will first get picked up by the owning scene
	CompileMeshData();
}

Room::~Room()
{
	delete roomCamera;
}

std::vector<Light>& Room::GetPointLights()
{
	return pointLights;
}

std::vector<DirectionalLight> Room::GetDirectionalLights() const
{
	return dirLights;
}

std::vector<RigidEntity>& Room::GetRigids()
{
	return rigids;
}

std::vector<StaticEntity>& Room::GetStatics()
{
	return statics;
}

std::vector<puzzleNode> Room::GetNodes() const
{
	return nodes;
}

std::vector<Mesh> Room::GetMeshData() const
{
	return meshes;
}

Camera* Room::GetCamera()
{
	return roomCamera;
}

//=============================================================
//	Room updates
//	Everything that updates in a room happens here. 
//	This can include collisions, camera movement,
//	character collision, etc.
//=============================================================
void Room::Update(Character* playerCharacter, GLFWwindow* renderWindow, float deltaTime)
{
	roomCamera->FPSCamControls(renderWindow, deltaTime);

	playerCharacter->SetColliding(false);
	RigidGroundCollision(playerCharacter);
	PlayerBoxCollision(playerCharacter);
	RigidRigidCollision();
	RigidNodeCollision();
	RigidStaticCollision();


	// Box holding
	if (playerCharacter->GetEntityID() >= 0)
	{
		if (playerCharacter->CheckInBound(rigids[playerCharacter->GetEntityID()]))
		{
			if (glfwGetKey(renderWindow, GLFW_KEY_L) == GLFW_PRESS)
			{
				rigids[playerCharacter->GetEntityID()].AddVelocity(playerCharacter->GetInputVector());
				rigids[playerCharacter->GetEntityID()].SetHeld(true);
			}
		}
		else
		{
			rigids[playerCharacter->GetEntityID()].SetHeld(false);
		}
	}
	playerCharacter->SetEntityID(inBoundCheck(*playerCharacter));
}

//=============================================================
//	Room updates
//	Check all rigid collisions with the ground, including player.
//	This loop trough all the rigids and all the statics in the scene.
//	It then does a collision check and applies a highest ground level
//	depending on which static is being collided with.
//=============================================================
void Room::RigidGroundCollision(Character* playerCharacter)
{
	// Rigid entites ground collision
	for (int i = 0; i < rigids.size(); i++)
	{
		// Recheck grounded state, assume it's not grounded
		rigids[i].SetGrounded(false);

		// Variable to find the highest ground level
		float ground = rigids[i].GetGroundLevel();

		// All the statics
		for (int j = 0; j < statics.size(); ++j)
		{
			if (rigids[i].CheckCollision(statics[j]))
			{
				// If this ground is higher use it
				ground = statics[j].GetHitboxTop();

				// This entity has been confirmed grounded
				rigids[i].SetGrounded(true);
			}
		}
		// All the bridges
		for (int j = 0; j < bridges.size(); ++j)
		{
			if (rigids[i].CheckCollision(bridges[j]))
			{
				// If this ground is higher use it
				ground = bridges[j].GetHitboxTop();

				// This entity has been confirmed grounded
				rigids[i].SetGrounded(true);
			}
		}

		rigids[i].GroundLevel(ground);
	}

	// Player ground collisions
	// Recheck grounded state, assume it's not grounded
	playerCharacter->SetGrounded(false);
	// Variable to find the highest ground level
	float ground = playerCharacter->GetGroundLevel();
	for (int j = 0; j < statics.size(); ++j)
	{
		if (playerCharacter->CheckCollision(statics[j]))
		{
	
			ground = statics[j].GetHitboxTop();
			playerCharacter->SetGrounded(true);
	
		}
	}

	for (int j = 0; j < bridges.size(); ++j)
	{
		if (playerCharacter->CheckCollision(bridges[j]))
		{

			ground = bridges[j].GetHitboxTop();
			playerCharacter->SetGrounded(true);
			
		}
	}
	playerCharacter->GroundLevel(ground);
}

//=============================================================
//	Room updates
//	Checks collision from the player to each box in the room
//=============================================================
void Room::PlayerBoxCollision(Character* playerCharacter)
{
	for (int i = 0; i < rigids.size(); ++i)
	{
		if (!rigids[i].IsHeld() && playerCharacter->CheckCollision(rigids[i]))
		{
			playerCharacter->SetColliding(true);

			// Push direction vector
			glm::vec3 pushDir = rigids[i].GetPosition() - playerCharacter->GetPosition();

			// Normalize and lock to 1 axis
			if (abs(pushDir.x) >= abs(pushDir.z))
				pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
			else
				pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
			pushDir *= 2.0f;

			// Add box velocity
			rigids[i].AddVelocity(pushDir);
		}
	}
}

int Room::inBoundCheck(Character playerCharacter)
{
	for (int i = 0; i < rigids.size(); i++)
		if (playerCharacter.CheckInBound(rigids[i]))
			return i;

	return -1;
}

//=============================================================
//	Room updates
//	Checks collision from the a rigid to each other rigid in the room
//=============================================================
void Room::RigidRigidCollision()
{
	// Could possibly be done with recursion to check subsequent collisions
	// Could be made better with proper physic calculations
	for (int i = 0; i < rigids.size(); ++i)
	{
		for (int j = 0; j < rigids.size(); ++j)
		{
			if (i != j && !rigids[j].IsHeld() && rigids[i].CheckCollision(rigids[j]))
			{
				if (!rigids[j].IsColliding())
				{
					// Push direction vector
					glm::vec3 pushDir = rigids[j].GetPosition() - rigids[i].GetPosition();

					// Normalize and lock to 1 axis
					if (abs(pushDir.x) >= abs(pushDir.z))
						pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
					else
						pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
					pushDir *= 2.0f;

					// Add box velocity
					rigids[j].AddVelocity(pushDir);
				}
			}
		}
	}
}

//=============================================================
//	Room updates
//	Checks collision from a node to each other entity in the room
//=============================================================
void Room::RigidNodeCollision()
{
	for (int i = 0; i < rigids.size(); i++)
	{
		if (nodes[0].CheckCollision(rigids[i]))
		{
			for (int j = 0; j < rigids.size(); ++j)
			{
				cout << "Solved" << endl;
			}
		}
	}
}

//=============================================================
//	Room updates
//	Checks collision from a static to each other rigid in the room
//=============================================================
void Room::RigidStaticCollision()
{
	for (int i = 0; i < rigids.size(); i++)
	{
		for (int j = 0; j < rigids.size(); ++j)
		{
			if (rigids[i].CheckCollision(rigids[j]))
			{

			}
			else
			{

			}
		}
	}

}

//=============================================================
//	Render update
//	Compiles mesh data for the renderer
//=============================================================
void Room::CompileMeshData()
{
	meshes.clear();

	for (int i = 0; i < rigids.size(); i++)
	{
		meshes.push_back(rigids[i].GetMeshData());
	}

	for (int i = 0; i < statics.size(); i++)
	{
		meshes.push_back(statics[i].GetMeshData());
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		meshes.push_back(nodes[i].GetMeshData());
	}

	for (int i = 0; i < bridges.size(); i++)
	{
		meshes.push_back(bridges[i].GetMeshData());
	}
}

//=============================================================
//	Light initialization
//	Loads and positions all the lights in the scene
//=============================================================
void Room::LoadLights()
{
	Light light;
	light.setDiffuse(glm::vec3(1.0f, 0.3f, 0.5f));
	light.setSpecular(glm::vec3(1.0f, 0.3f, 0.5f));

	light.setLightPos(glm::vec3(3.0f, 1.0f, -3.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 2.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 7.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, -3.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 2.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 7.0f));
	pointLights.push_back(light);

	DirectionalLight light2;
	dirLights.push_back(light2);
}

//=============================================================
//	Entity initialization
//	Loads and positions all the entities in the scene
//=============================================================
void Room::LoadEntities(std::vector<Material> materials)
{
	// Temporary Loader and meshes
	Loader testLoader("TryCubeFrozenBinary.bin");

	// Uses the first slot of the testLoader file which is currently a cube "xTestBinary4.bin"
	RigidEntity newEntity(testLoader.getVerticies(0), testLoader.getNrOfVerticies(0));
	newEntity.SetMaterialID(materials[0].getMaterialID());
	rigids.push_back(newEntity);

	RigidEntity cubeEntity(1);
	cubeEntity.SetMaterialID(materials[0].getMaterialID());

	cubeEntity.SetPosition(glm::vec3(3.0f, 10.0f, -3.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(3.0f, 100.0f, 2.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(3.0f, 1.0f, 7.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(-3.0f, 1.0f, -3.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(-3.0f, 1.0f, 2.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(-3.0f, 1.0f, 7.0f));
	rigids.push_back(cubeEntity);
	
	StaticEntity planeEntity(0);
	planeEntity.SetMaterialID(materials[0].getMaterialID());
	planeEntity.SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));
	statics.push_back(planeEntity);

	BridgeEntity bridge1(1);

	bridge1.SetMaterialID(materials[2].getMaterialID());
	bridge1.SetPosition(glm::vec3(11.0f, -0.5f, 0.0f));
	bridge1.SetRestPosition(glm::vec3(11.0f, -0.5f, 0.0f));
	bridges.push_back(bridge1);

	bridge1.SetMaterialID(materials[2].getMaterialID());
	bridge1.SetPosition(glm::vec3(12.0f, 0.2f, 0.0f));
	bridge1.SetRestPosition(glm::vec3(12.0f, 0.2f, 0.0f));
	bridges.push_back(bridge1);

	bridge1.SetMaterialID(materials[2].getMaterialID());
	bridge1.SetPosition(glm::vec3(13.0f, 0.4f, 0.0f));
	bridge1.SetRestPosition(glm::vec3(13.0f, 0.4f, 0.0f));
	bridges.push_back(bridge1);

	bridge1.SetMaterialID(materials[2].getMaterialID());
	bridge1.SetPosition(glm::vec3(14.0f, 0.6f, 0.0f));
	bridge1.SetRestPosition(glm::vec3(14.0f, 0.6f, 0.0f));
	bridges.push_back(bridge1);

}

//=============================================================
//	Puzzle node initialization
//	Loads and positions all the puzzle nodes in the scene
//=============================================================
void Room::LoadPuzzleNode(std::vector<Material> materials)
{
	puzzleNode winNode;
	winNode.SetMaterialID(materials[3].getMaterialID());
	winNode.SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	nodes.push_back(winNode);
}

