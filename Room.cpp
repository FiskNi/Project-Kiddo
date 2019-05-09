#include "Room.h"



Room::Room(std::vector<Material> materials, Loader* aLoader)
{
	LoadLights();
	LoadEntities(materials, aLoader);
	LoadPuzzleNode(materials);
	isRoomCompleted = false;

	// Initialize camera (Default constructor)
	roomCamera = new Camera;

	// Compiles all the mesh data in the room for the renderer
	CompileMeshData();
}

//Room::Room(std::vector<Material> materials, int state)
//{
//	// This is to initialise the Main Menu room scene
//
//	// Check which state is active, and run loading accordingly
//	if (state == MAINMENU) 
//	{
//		// Hardcoded quad to print something to the screen
//		RigidEntity quad(0);
//		quad.SetPosition(glm::vec3(-8.0f, 5.0f, 3.0f));
//		quad.SetMaterialID(materials[0].getMaterialID());
//		quad.SetStartPosition(glm::vec3(-8.0f, 5.0f, 3.0f));
//		rigids.push_back(quad);
//
//		// Perhaps change position for the menu?
//		// Initialize camera (Default constructor)
//		roomCamera = new Camera;
//	}
//
//	// Compiles all the mesh data in the room for the renderer
//	CompileMeshData();
//}

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

	// Reset collisions
	playerCharacter->SetColliding(false);
	for (int i = 0; i < rigids.size(); i++)
		rigids[i].SetColliding(false);


	BoxHolding(playerCharacter, renderWindow);

	RigidGroundCollision(playerCharacter);
	PlayerRigidCollision(playerCharacter);
	RigidRigidCollision();
	RigidNodeCollision();
	RigidStaticCollision(playerCharacter);
	BridgeUpdates(renderWindow);
	BoxPlateCollision(playerCharacter);
	ButtonInteract(renderWindow, playerCharacter);


	// Game events
	// This is where link IDs will be added for each entity in the scene based on importer attributes
	/*if (plates[0].isPressed())
	{
		bridges[0].Extend();		
	}
	else
	{
		bridges[0].Retract();
	}

	if (plates[1].isPressed())
	{
		bridges[1].Extend();
	}
	else
	{
		bridges[1].Retract();
	}

	if (buttons[0].isPressed())
	{
		bridges[2].Extend();
	}
	else
	{
		bridges[2].Retract();
	}*/

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

void Room::BoxPlateCollision(Character* playerCharacter)
{
	//CHANGE COLLISION TO NOT JUST BE TOUCH BUT OVERALL ON TOP OF 
	for (int i = 0; i < plates.size(); i++) 
	{
		plates[i].setPressed(false);
		for (int j = 0; j < rigids.size(); j++)
		{
			if (!plates[i].isPressed() && rigids[j].CheckCollision(plates[i]) && plates[i].CheckInsideCollision(rigids[j]))
			{
				plates[i].setPressed(true);
			}
		}

		if (!plates[i].isPressed() && playerCharacter->CheckCollision(plates[i]) && plates[i].CheckInsideCollision(*playerCharacter))
		{
			plates[i].setPressed(true);
		}

	}
}

//=============================================================
//	Checks all rigid collisions with the ground, includes the player.
//	This loops trough all the rigids and all the statics in the scene.
//	Afterwards does a collision check and applies the highest ground level found.
//	The actual action on collison happens in the rigid entity class.
//=============================================================
void Room::ButtonInteract(GLFWwindow* window, Character * playerCharacter)
{
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (!buttons[i].isPressed() && playerCharacter->CheckCollision(buttons[i]))
			{
				buttons[i].setPressed(true);
			}
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
//	Checks all rigid collisions with the ground, includes the player.
//	This loops trough all the rigids and all the statics in the scene.
//	Afterwards does a collision check and applies the highest ground level found.
//	The actual action on collison happens in the rigid entity class.
//=============================================================
void Room::RigidGroundCollision(Character* playerCharacter)
{
	const float maxDiff = 1.0f; // Max ground height difference

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
				// If ground is close enough
				if (abs(rigids[i].GetHitboxBottom() - statics[j].GetHitboxTop()) < maxDiff)
				{
					ground = statics[j].GetHitboxTop();
					rigids[i].SetGrounded(true);
				}
			}
		}

		// All the bridges
		for (int j = 0; j < bridges.size(); ++j)
		{
			if (rigids[i].CheckCollision(bridges[j]))
			{	
				// If ground is close enough
				if (abs(rigids[i].GetHitboxBottom() - bridges[j].GetHitboxTop()) < maxDiff)
				{
					ground = bridges[j].GetHitboxTop();
					rigids[i].SetGrounded(true);
				}
			}
		}

		//All the boxHolders
		for (int j = 0; j < holders.size(); ++j)
		{
			if (rigids[i].CheckHolderCollision(holders[j]))
			{
				// If ground is close enough
				if (abs(rigids[i].GetHitboxBottom() - holders[j].GetHitboxTopOffsetBB()) < maxDiff)
				{
					if (holders[j].getVisible() == false)
					{
						holders[j].setVisible(true);
						holders[j].boxReturn();
						rigids[i].SetPosition(glm::vec3(999, 0, 0));
					}
					ground = holders[j].GetHitboxTopOffsetBB();
					rigids[i].SetGrounded(true);
				}
			}
		}

		rigids[i].GroundLevel(ground);
	}

	// Player ground collisions
	// Recheck grounded state, assume it's not grounded
	playerCharacter->SetGrounded(false);
	float ground = playerCharacter->GetGroundLevel();
	for (int j = 0; j < statics.size(); ++j)
	{
		if (playerCharacter->CheckCollision(statics[j]))
		{
			// If ground is close enough
			if (abs(playerCharacter->GetHitboxBottom() - statics[j].GetHitboxTop()) < maxDiff)
			{
				ground = statics[j].GetHitboxTop();
				playerCharacter->SetGrounded(true);
			}	
		}
	}

	for (int j = 0; j < bridges.size(); ++j)
	{
		if (playerCharacter->CheckCollision(bridges[j]))
		{
			// If ground is close enough
			if (abs(playerCharacter->GetHitboxBottom() - bridges[j].GetHitboxTop()) < maxDiff)
			{
				ground = bridges[j].GetHitboxTop();
				playerCharacter->SetGrounded(true);
			}	
		}
	}

	for (int j = 0; j < holders.size(); ++j)
	{
		if (holders[j].getVisible() == true)
		{
			if (playerCharacter->CheckHolderCollision(holders[j]))
			{
				// If ground is close enough
				if (abs(playerCharacter->GetHitboxBottom() - holders[j].GetHitboxTopOffsetBB()) < maxDiff)
				{
					//holders[j].puntBox();
					ground = holders[j].GetHitboxTopOffsetBB();
					playerCharacter->SetGrounded(true);
				}
			}
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
			
			// Push direction vector
			glm::vec3 pushDir = rigids[i].GetPosition() - playerCharacter->GetPosition();
			pushDir = glm::normalize(pushDir);

			// Lock to 1 axis
			if (abs(pushDir.x) >= abs(pushDir.z))
				pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
			else
				pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
			pushDir *= 1.5f;

			// Add box velocity
			rigids[i].AddVelocity(pushDir);

			// This always comes in as false so this if state doesn't work but is a template
			// for a possible solution. To be deleted later if not used.
			if (playerCharacter->IsColliding())
			{
				playerCharacter->AddVelocity(-pushDir);
			}
			else
			{
				playerCharacter->SetColliding(true);
				playerCharacter->SetPosition(playerCharacter->GetSavedPos());
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
					pushDir = normalize(pushDir);

					// Lock to 1 axis
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
//	Checks collision from all entities to all nodes in the room
//=============================================================
void Room::RigidNodeCollision()
{
	if (isRoomCompleted != true) {
		for (int i = 0; i < rigids.size(); i++)
		{
			if (nodes[0].CheckCollision(rigids[i]))
			{
				for (int j = 0; j < rigids.size(); ++j)
				{
					cout << "Solved" << endl;
					isRoomCompleted = true;
				}
			}
		}
	}
}

//=============================================================
//	Checks collision from all rigids to all statics in the room
//=============================================================
void Room::RigidStaticCollision(Character* playerCharacter)
{
	for (int i = 0; i < rigids.size(); i++)
	{
		for (int j = 0; j < statics.size(); ++j)
		{
			if (rigids[i].CheckCollision(statics[j]))
			{
				if (abs(statics[j].GetHitboxTop() - rigids[i].GetHitboxBottom()) >= 0.5f)
				{
					glm::vec3 pushDir = statics[i].GetPosition() - rigids[i].GetPosition();
					pushDir = normalize(pushDir);

					// Lock to 1 axis
					if (abs(pushDir.x) >= abs(pushDir.z))
						pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
					else
						pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
					pushDir *= 2.0f;

					rigids[i].SetPosition(rigids[i].GetSavedPos());
				}
			}
		}
	}


	for (int i = 0; i < statics.size(); ++i)
	{
		if (playerCharacter->CheckCollision(statics[i]))
		{
			if (abs(statics[i].GetHitboxTop() - playerCharacter->GetHitboxBottom()) >= 0.5f)
			{
				glm::vec3 pushDir = statics[i].GetPosition() - playerCharacter->GetPosition();
				pushDir = normalize(pushDir);

				// Lock to 1 axis
				if (abs(pushDir.x) >= abs(pushDir.z))
					pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
				else
					pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
				pushDir *= 2.0f;

				playerCharacter->AddVelocity(-pushDir);
				playerCharacter->SetPosition(playerCharacter->GetSavedPos());
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

void Room::destroyRoom()
{
	delete roomCamera;
}


//=============================================================
//	Compiles mesh data for the renderer
//=============================================================
void Room::CompileMeshData()
{
	// NEEDS TO BE CHANGED SO THE VECTOR DOESNT REALLOCATED ALL THE TIME
	meshes.clear();

	for (int i = 0; i < roomMeshes.size(); i++)
	{
		meshes.push_back(roomMeshes[i]);
	}

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

	for (int i = 0; i < plates.size(); i++) 
	{
		meshes.push_back(plates[i].GetMeshData());
	}

	for (int i = 0; i < buttons.size(); i++) 
	{
		meshes.push_back(buttons[i].GetMeshData());
	}

	for (int i = 0; i < holders.size(); i++)
	{
		meshes.push_back(holders[i].GetMeshData());
		meshes.push_back(holders[i].GetHolderMeshData());
	}
}

//=============================================================
//	Light initialization
//	Loads and positions all the lights in the scene
//=============================================================
void Room::LoadLights()
{
	Light light(3.0f, 1.0f, -3.0f, 15, 160, 8);
	light.setDiffuse(glm::vec3(1.0f, 1.0, 1.0f));
	light.setSpecular(glm::vec3(0.0f, 0.2f, 0.8f));

	light.setLightPos(glm::vec3(8.0f, 2.0f, -3.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(8.0f, 2.0f, 2.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(8.0f, 2.0f, 7.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-8.0f, 2.0f, -3.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-8.0f, 2.0f, 2.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-8.0f, 2.0f, 7.0f));
	pointLights.push_back(light);

	DirectionalLight light2;
	dirLights.push_back(light2);
}

//=============================================================
//	Entity initialization
//	Loads and positions all the entities in the scene
//=============================================================
void Room::LoadEntities(std::vector<Material> materials, Loader* level)
{

	//==========
	// Entity loading will be changed to take in custom attributes and base what is loaded into the room on these
	// Will need to move the Loader up from this location to properly take in materials as well
	// The pipeline needs to be looked over in general to determine how things will load and be created
	//==========

	for (int i = 0; i < level->GetMeshCount(); i++)
	{
		//Custom attributes to be detected here before pushed into the appropriate category?
		switch (level->GetType(i))
		{
		case 0:		// Mesh
			{
				Mesh mesh(level->GetVerticies(i), level->GetVertexCount(i), materials[0].getMaterialID());
				roomMeshes.push_back(mesh);
			}
			break;
		case 1:		// Mesh
			{
				//Mesh mesh(level->GetVerticies(i), level->GetVertexCount(i), materials[0].getMaterialID());
				//roomMeshes.push_back(mesh);
			}
			break;

		case 2:		// Static
			{
				StaticEntity levelEntity(level, i, materials[0].getMaterialID());
				statics.push_back(levelEntity);
			}
			break;

		case 3:		// Rigid
			{
				RigidEntity cubeEntity(level, i, materials[0].getMaterialID());
				rigids.push_back(cubeEntity);
			}
			break;

		case 4:		// Bridge
			{
				Mesh mesh(level->GetVerticies(i), level->GetVertexCount(i), materials[0].getMaterialID());
				roomMeshes.push_back(mesh);
			}
			break;

		case 5:		// DrawBridge
			{
				BridgeEntity bridgeEntity(level, i, materials[2].getMaterialID());
				// Needs to be looked over, might need values from maya
				bridgeEntity.SetExtendingBackwardZ();
				bridgeEntity.SetExtendDistance(4.2f);
				bridges.push_back(bridgeEntity);
			}
			break;

		case 6:		// Button
			{
				Button button;
				button.SetMaterialID(materials[0].getMaterialID());
				button.scaleBB(2);
				buttons.push_back(button);
			}
			break;

		case 7:		// Presure Plate
			{
				PressurePlate pPlate;
				pPlate.SetMaterialID(materials[1].getMaterialID());
				pPlate.setBBY(2.0f);
				pPlate.scaleBB(2.0f);
				plates.push_back(pPlate);
			}
			break;

		case 8:		// Character
			break;

		case 9:		// Door
			{
				Mesh mesh(level->GetVerticies(i), level->GetVertexCount(i), materials[0].getMaterialID());
				roomMeshes.push_back(mesh);
			}
			break;

		case 10:	// Plushie
			break;

		case 11:	// Light
			break;

		case 12:	// Collectible
			break;

		case 13:	// Box holder
			{
				boxHolder boxHolderEntity(level, i, materials[0].getMaterialID(), materials[0].getMaterialID());
				boxHolderEntity.puntBox();
				this->holders.push_back(boxHolderEntity);
			}
			break;

		default:
			break;
		}

	

	}
}

//=============================================================
//	Puzzle node initialization
//	Loads and positions all the puzzle nodes in the scene
//=============================================================
void Room::LoadPuzzleNode(std::vector<Material> materials)
{
	puzzleNode winNode;
	winNode.SetMaterialID(materials[3].getMaterialID());
	winNode.SetPosition(glm::vec3(-5.0f, -0.4f, -9.0f));
	nodes.push_back(winNode);
}

