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
	PlayerBoxCollision(playerCharacter);
	RigidRigidCollision();
	RigidNodeCollision();
	RigidStaticCollision();
	RigidGroundCollision(playerCharacter);

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

void Room::RigidGroundCollision(Character* playerCharacter)
{
	// Entity boxes
	for (int i = 0; i < rigids.size(); i++)
	{
		rigids[i].SetGrounded(false);
		for (int j = 0; j < statics.size(); ++j)
		{
			if (rigids[i].CheckCollision(statics[j]))
			{
				// Cancel downwards movement
				rigids[i].SetGrounded(true);
				rigids[i].SetVelocityY(0.0f);

				// Moves the entity back up if it's below the ground
				// Could be improved by larger physics calculations
				float groundY = statics[j].GetPositionBB().y;
				float offset = groundY - rigids[i].GetHitboxBottom();
				offset *= 20.0f;
				rigids[i].AddVelocityY(offset);
			}
		}
	}

	// Player
	playerCharacter->SetGrounded(false);
	for (int i = 0; i < rigids.size(); i++)
	{
		for (int j = 0; j < statics.size(); ++j)
		{
			if (playerCharacter->CheckCollision(statics[j]))
			{
				// Cancel downwards movement
				playerCharacter->SetGrounded(true);
				playerCharacter->SetVelocityY(0.0f);

				// Moves the entity back up if it's below the ground
				// Could be improved by larger physics calculations
				float groundY = statics[j].GetHitboxTop();
				float offset = groundY - playerCharacter->GetHitboxBottom();
				offset *= 20.0f;
				playerCharacter->AddVelocityY(offset);
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

	meshes.push_back(this->importMeshes[0]);
	meshes.push_back(this->importMeshes[1]);
	meshes.push_back(this->importMeshes[2]);

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
	Loader testLoader("xTestBinary4.bin");
	Mesh testMesh;
	Mesh test2;
	Mesh test3;

	testMesh.setPosition(glm::vec3(-6.0f, 3.0f, 10.0f));
	testMesh.setMaterial(materials[0].getMaterialID());

	test2.setPosition(glm::vec3(-12, 3.0, 10.0f));
	test2.setMaterial(materials[0].getMaterialID());

	test3.setPosition(glm::vec3(-18.0f, 3.0f, 10.0f));
	test3.setMaterial(materials[0].getMaterialID());

	this->importMeshes.push_back(testMesh);
	this->importMeshes.push_back(test2);
	this->importMeshes.push_back(test3);

	for (int i = 0; i < testLoader.getNrOfMeshes(); i++)
	{
		this->importMeshes[i].ImportMesh(testLoader.getVerticies(i), testLoader.getNrOfVerticies(i));
	}

	// Uses the first slot of the testLoader file which is currently a cube "xTestBinary4.bin"
	RigidEntity cubeEntity(testLoader.getVerticies(0), testLoader.getNrOfVerticies(0));
	cubeEntity.SetMaterialID(materials[0].getMaterialID());

	cubeEntity.SetPosition(glm::vec3(3.0f, 1.0f, -3.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(3.0f, 1.0f, 2.0f));
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
	bridge1.SetPosition(glm::vec3(11.0f, -1.0f, 0.0f));
	bridge1.SetRestPosition(glm::vec3(11.0f, -1.0f, 0.0f));

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

