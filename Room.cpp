#include "Room.h"



Room::Room(std::vector<Material> materials)
{
	LoadLights();
	LoadEntities(materials);
	LoadPuzzleNode(materials);


	// Initialize camera (Default constructor)
	roomCamera = new Camera;

	// Compiles all the mesh data in the room for the renderer
	CompileMeshData();
}

Room::~Room()
{
	delete roomCamera;
}

//=============================================================
//	Everything that updates in a room happens here. 
//	This can include collisions, camera movement,
//	character collision, etc.
//=============================================================
void Room::Update(Character* playerCharacter, GLFWwindow* renderWindow, float deltaTime)
{
	roomCamera->FPSCamControls(renderWindow, deltaTime);

	playerCharacter->SetColliding(false);
	BoxHolding(playerCharacter, renderWindow);

	for (int i = 0; i < rigids.size(); i++)
	{
		if (rigids[i].isCollidingStatic())
		{
			//If it's not moving it's no longer colliding with the static
			if (rigids[i].GetVelocity() == glm::vec3(0, 0, 0))
			{
				rigids[i].SetCollidingStatic(false);
			}
		}
	}

	RigidGroundCollision(playerCharacter);
	PlayerRigidCollision(playerCharacter);
	RigidRigidCollision();
	RigidNodeCollision();
	RigidStaticCollision();
	BridgeUpdates(renderWindow);
	BoxPlateCollision();
}

//=============================================================
//	Picks up a box in range. Currently uses the vector list as priority
//	if multiple boxes are in range. 
//	Can be modified to look for the closest box.
//=============================================================
void Room::BoxHolding(Character* playerCharacter, GLFWwindow* renderWindow)
{
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
void Room::BoxPlateCollision()
{
	//CHANGE COLLISION TO NOT JUST BE TOUCH BUT OVERALL ON TOP OF 
	for (int i = 0; i < plates.size(); i++) {
		bool pressed = false;

		if (!plates[i].isPressed()) {
			for (int j = 0; j < rigids.size(); j++) {
				if (!pressed) {
					if (rigids[j].CheckCollision(plates[i])) {
						if (plates[i].CheckInsideCollision(rigids[j])) {
							pressed = true;
						}
					}
				}
			}
		}

		plates[i].setPressed(pressed);
		if (pressed) std::cout << "hi" << std::endl;
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
//	Checks all rigid collisions with the ground, includes the player.
//	This loops trough all the rigids and all the statics in the scene.
//	Afterwards does a collision check and applies the highest ground level found.
//	The actual action on collison happens in the rigid entity class.
//=============================================================
void Room::RigidGroundCollision(Character* playerCharacter)
{
	 //Rigid entites ground collision
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
	// System for this currently not implemented.
	// Right now the objects further back in the vector will be dominating the ground variable.
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
//	Checks collision from the player to all rigids in the room
//	Adds velocity to the box being collided with unless the box is
//	being held.
//=============================================================
void Room::PlayerRigidCollision(Character* playerCharacter)
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
			if (!rigids[i].isCollidingStatic())
			{
				rigids[i].AddVelocity(pushDir);
			}
			else
			{
				pushDir = normalize(pushDir);
				playerCharacter->SetVelocity(-pushDir*0.2f);
			}
		}
	}
}



//=============================================================
//	Checks collision from all rigids to each other rigid in the room.
//	Boxes will mutually push each other away from one another on collision.
//	Locked to the x and y world axis.
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

					//if (!rigids[i].isCollidingStatic())
					//{
					//	rigids[i].AddVelocity(pushDir);
					//}
					//else
					//{
					//	pushDir = normalize(pushDir);
					//	//Switch with one of the boxes?
					//	playerCharacter->SetVelocity(-pushDir * 0.2f);
					//}
				}
			}
		}
	}
}

//=============================================================
//	Checks collision from all entities to all nodes in the room
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
//	Checks collision from all rigids to all statics in the room
//=============================================================
void Room::RigidStaticCollision()
{
	for (int i = 0; i < rigids.size(); i++)
	{
		for (int j = 0; j < statics.size(); ++j)
		{
			if (rigids[i].CheckCollision(statics[j]))
			{
				// Alternative, are we colliding with top or bottom? if not then we are colliding 
				// with sides and should no longer be able to move in the direction we were
				// How stop that? Either jump back to position frame before(probably laggy) or 
				// wall push back with exact opposite intensity should kill movement.
				// For every single action there's an equal, opposite reaction.
				glm::vec3 pushDir = statics[j].GetPosition() - rigids[i].GetPosition();

				//The reverse direction from the "Wall"
				glm::vec3 revDir = glm::normalize(-pushDir);

				if (rigids[i].GetGroundLevel() != statics[j].GetHitboxTop())
				{
					if (!rigids[i].isCollidingStatic())
					{
						// Normalize and lock to 1 axis
						if (abs(pushDir.x) >= abs(pushDir.z))
							pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
						else
							pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);

						cout << "rigid" << i << "STATIC COLLISH"<< endl;
						rigids[i].AddVelocity(-pushDir);
						//rigids[i].setForbiddenDir(normalize(pushDir));
						rigids[i].SetCollidingStatic(true);
					}
					//else
					//{
					//	if (rigids[i].GetVelocity() == glm::vec3(0, 0, 0))
					//	{
					//		cout << "collish false" << endl;
					//		rigids[i].SetCollidingStatic(false);
					//	}
					//}
				}

			}
			else
			{
				//Can't be done here since the rigid is constantly NOT colliding with every other static in the scene.
				//rigids[i].SetCollidingStatic(false);
			}
		}
	}

}

void Room::BridgeUpdates(GLFWwindow *renderWindow)
{
	for (int i = 0; i < bridges.size(); i++)
	{
		// Template for the updates, this can be replaced by whatever event
		if (bridges[i].CheckLinkID(-999) && glfwGetKey(renderWindow, GLFW_KEY_H) == GLFW_PRESS)
		{
			bridges[i].Extend();
		}
		else if(bridges[i].CheckLinkID(-999) && glfwGetKey(renderWindow, GLFW_KEY_N) == GLFW_PRESS)
		{
			bridges[i].Retract();
		}
	}
}


//=============================================================
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

	for (int i = 0; i < plates.size(); i++) {
		meshes.push_back(plates[i].GetMeshData());
	}
}

//=============================================================
//	Light initialization
//	Loads and positions all the lights in the scene
//=============================================================
void Room::LoadLights()
{
	Light light;
	light.setDiffuse(glm::vec3(0.0f, 1.0, 0.8f));
	light.setSpecular(glm::vec3(0.0f, 0.2f, 0.8f));

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
	StaticEntity newEntity(testLoader.getVerticies(0), testLoader.getNrOfVerticies(0));
	newEntity.SetMaterialID(materials[0].getMaterialID());
	newEntity.SetPositionY(-1.2f);
	statics.push_back(newEntity);

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
	//statics.push_back(planeEntity);

	Loader level("LevelTest.bin");

	for (int i = 0; i < level.getNrOfMeshes(); i++)
	{
		StaticEntity levelEntity(level.getVerticies(i), level.getNrOfVerticies(i));
		levelEntity.SetMaterialID(materials[0].getMaterialID());
		levelEntity.OffsetPositionY(1.2f);
		this->statics.push_back(levelEntity);
	}

	BridgeEntity bridge1(1);

	bridge1.SetMaterialID(materials[2].getMaterialID());
	//bridge1.SetPosition(glm::vec3(-5.0f, -0.5f, 0.0f)); // This doesnt matter while the update function is running
	bridge1.SetRestPosition(glm::vec3(-5.0f, -0.5f, 0.0f));
	bridges.push_back(bridge1);

	PressurePlate plate;
	plate.SetMaterialID(materials[2].getMaterialID());
	plate.SetPosition(glm::vec3(0, -1, 0));
	plate.setBBY(0.9);
	plate.scaleBB(1.3);
	plates.push_back(plate);

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

