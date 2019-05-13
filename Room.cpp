#include "Room.h"



Room::Room(std::vector<Material> materials, Loader* aLoader)
{
	LoadLights(aLoader);
	LoadEntities(materials, aLoader);
	LoadPuzzleNode(materials);
	isRoomCompleted = false;

	// Initialize camera (Default constructor)
	roomCamera = new Camera;

	// Compiles all the mesh data in the room for the renderer
	CompileMeshData();
}

Room::~Room()
{
	if (roomCamera)
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
	PlayerItemCollision(playerCharacter);

	// Game events
	// This is where link IDs will be added for each entity in the scene based on importer attributes
	for (int i = 0; i < pressurePlates.size(); i++)
	{
		if (pressurePlates[i].isPressed())
		{
			for (int j = 0; j < bridges.size(); j++)
			{
				if (bridges[j].CheckLinkID(pressurePlates[i].GetLinkID()))
					bridges[j].Extend();
			}
		}
		else
		{
			for (int j = 0; j < bridges.size(); j++)
			{
				if (bridges[j].CheckLinkID(pressurePlates[i].GetLinkID()))
					bridges[j].Retract();
			}
		}
	}

	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].isPressed())
		{
			for (int j = 0; j < bridges.size(); j++)
			{
				if (bridges[j].CheckLinkID(buttons[i].GetLinkID()))
					bridges[j].Extend();
			}
		}
		else
		{
			for (int j = 0; j < bridges.size(); j++)
			{
				if (bridges[j].CheckLinkID(buttons[i].GetLinkID()))
					bridges[j].Retract();
			}
		}
	}

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
	for (int i = 0; i < pressurePlates.size(); i++) 
	{
		pressurePlates[i].setPressed(false);
		for (int j = 0; j < rigids.size(); j++)
		{
			if (!pressurePlates[i].isPressed() && rigids[j].CheckCollision(pressurePlates[i]) && pressurePlates[i].CheckInsideCollision(rigids[j]))
			{
				pressurePlates[i].setPressed(true);
			}
		}

		if (!pressurePlates[i].isPressed() && playerCharacter->CheckCollision(pressurePlates[i]) && pressurePlates[i].CheckInsideCollision(*playerCharacter))
		{
			pressurePlates[i].setPressed(true);
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
	/*if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (!buttons[i].isPressed() && playerCharacter->CheckCollision(buttons[i]))
			{
				buttons[i].SetPressed(true);
			}
		}
	}*/
}

void Room::PlayerItemCollision(Character* playerCharacter)
{
	for (int i = 0; i < items.size(); i++) {
		if (playerCharacter->CheckCollision(items[i])) {
			playerCharacter->PickUpItem(&items[i]);
			items[i].SetPickedUp(true);
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
	const float maxDiff = 0.5f; // Max ground height difference

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

					pushDir.y = 0.0f;
					pushDir *= 3.0f;

					rigids[i].SetPosition(rigids[i].GetSavedPos());
				}
			}
		}
		for (int j = 0; j < bridges.size(); ++j)
		{
			if (rigids[i].CheckCollision(bridges[j]))
			{
				if (abs(bridges[j].GetHitboxTop() - rigids[i].GetHitboxBottom()) >= 0.5f)
				{
					glm::vec3 pushDir = bridges[i].GetPosition() - rigids[i].GetPosition();
					pushDir = normalize(pushDir);

					pushDir.y = 0.0f;
					pushDir *= 3.0f;

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

				pushDir.y = 0.0f;
				pushDir *= 3.0f;

				playerCharacter->SetVelocity(-pushDir);
				playerCharacter->SetPosition(playerCharacter->GetSavedPos());
				playerCharacter->SetColliding(true);
			}
		}
	}

	for (int i = 0; i < bridges.size(); ++i)
	{
		if (playerCharacter->CheckCollision(bridges[i]))
		{
			if (abs(bridges[i].GetHitboxTop() - playerCharacter->GetHitboxBottom()) >= 0.5f)
			{
				glm::vec3 pushDir = bridges[i].GetPosition() - playerCharacter->GetPosition();
				pushDir = normalize(pushDir);

				pushDir.y = 0.0f;
				pushDir *= 3.0f;

				playerCharacter->SetVelocity(-pushDir);
				playerCharacter->SetPosition(playerCharacter->GetSavedPos());
				playerCharacter->SetColliding(true);
			}
		}
	}



}

void Room::BridgeUpdates(GLFWwindow *renderWindow)
{
	for (int i = 0; i < bridges.size(); i++)
	{
		// Template for the updates, this can be replaced by whatever event
		if (glfwGetKey(renderWindow, GLFW_KEY_H) == GLFW_PRESS)
		{
			bridges[i].Extend();
		}
		else if(glfwGetKey(renderWindow, GLFW_KEY_N) == GLFW_PRESS)
		{
			bridges[i].Retract();
		}
	}
}

void Room::destroyRoom()
{
	delete roomCamera;
}


void Room::Upgrade(Character* playerCharacter)
{
	Item* temp = playerCharacter->GetCurrentItem();
	if (temp != nullptr) {
		for (int i = 0; i < rigids.size(); i++) {
			if (playerCharacter->CheckInBound(rigids[i])) {
				rigids[i].SetBoxType(playerCharacter->GetCurrentItem()->GetItemType());
				if (rigids[i].GetBoxType() == 1) {
					std::cout << "box upgraded" << std::endl;
				}
				//playerCharacter->ItemUsed();

			}
		}
	}

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

	for (int i = 0; i < pressurePlates.size(); i++) 
	{
		meshes.push_back(pressurePlates[i].GetMeshData());
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
	for (int i = 0; i < items.size(); i++) {
		if (!items[i].GetPickedUp()) {
			meshes.push_back(items[i].GetMeshData());
		}
	}
}

//=============================================================
//	Light initialization
//	Loads and positions all the lights in the scene
//=============================================================
void Room::LoadLights(Loader* inLoader)
{
	Light light(0.0f, 0.0f, 0.0f, 0.0f, 200, 9);

	light.SetDiffuse(glm::vec3(1.0f, 1.0, 1.0f));
	light.SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));

	pointLights.push_back(light);
	pointLights.push_back(light);
	pointLights.push_back(light);
	pointLights.push_back(light);
	pointLights.push_back(light);
	pointLights.push_back(light);

	for (int i = 0; i < inLoader->GetPointLightCount(); i++)
	{
		glm::vec3 pos = glm::vec3(
				inLoader->GetPointLightPos(i)[0],
				inLoader->GetPointLightPos(i)[1],
				inLoader->GetPointLightPos(i)[2]);
		glm::vec3 color = glm::vec3(
			inLoader->GetPointLightColor(i)[0],
			inLoader->GetPointLightColor(i)[1],
			inLoader->GetPointLightColor(i)[2]);

		
		pointLights[i].setLightPos(pos);
		pointLights[i].SetDiffuse(color);
		pointLights[i].setPower(inLoader->GetPointLightIntensity(i) * 0.1f);
	}

	DirectionalLight dirLight;
	dirLights.push_back(dirLight);

	for (int i = 0; i < inLoader->GetDirLightCount(); i++)
	{
		glm::vec3 pos = glm::vec3(
			inLoader->GetDirLightPos(i)[0],
			inLoader->GetDirLightPos(i)[1],
			inLoader->GetDirLightPos(i)[2]);
		glm::vec3 color = glm::vec3(
			inLoader->GetDirLightColor(i)[0],
			inLoader->GetDirLightColor(i)[1],
			inLoader->GetDirLightColor(i)[2]);


		dirLights[i].SetPos(pos);
		dirLights[i].SetStrength(inLoader->GetDirLightIntensity(i) * 0.1f);
		dirLights[i].SetDiffuse(color);
	}

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
		unsigned int matID = level->GetMaterialID(i);
		switch (level->GetType(i))
		{
		case 0:		// Mesh
			{
				Mesh mesh(level, i);
				roomMeshes.push_back(mesh);
			}
			break;
		case 1:		// Mesh
			{
				Mesh mesh(level, i);
				roomMeshes.push_back(mesh);
			}
			break;

		case 2:		// Static
			{
				StaticEntity levelEntity(level, i, matID, true);
				statics.push_back(levelEntity);
			}
			break;

		case 3:		// Rigid
			{
				RigidEntity cubeEntity(level, i, matID, true);
				cubeEntity.OffsetPositionY(3.0f);
				cubeEntity.SetStartPosition(cubeEntity.GetPosition());
				rigids.push_back(cubeEntity);
			}
			break;

		case 4:		// Bridge
			{
				BridgeEntity bridgeEntity(level, i, matID, true);
				// Needs to be looked over, might need values from maya
				bridgeEntity.SetLinkID(level->GetMesh(i).link);
				bridgeEntity.SetExtendingDir(level->GetMesh(i).dir);
				bridgeEntity.SetExtendDistance(level->GetMesh(i).dist);
				bridges.push_back(bridgeEntity);
				Mesh mesh(level, i);
				roomMeshes.push_back(mesh);
			}
			break;

		case 5:		// Box Holder
			{
				boxHolder boxHolderEntity(level, i, matID, matID, true);
				boxHolderEntity.puntBox();
				this->holders.push_back(boxHolderEntity);
			}
			break;

		case 6:		// Button
			{
				Button button(level, i, matID, true);
				button.SetLink(level->GetMesh(i).link);
				button.SetMaterialID(matID);
				button.scaleBB(2);
				buttons.push_back(button);
			}
			break;

		case 7:		// Presure Plate
			{
				PressurePlate pPlate(level, i, matID, true);
				pPlate.SetLink(level->GetMesh(i).link);
				pPlate.SetMaterialID(matID);
				pPlate.setBBY(2.0f);
				pPlate.scaleBB(2.0f);
				pressurePlates.push_back(pPlate);
			}
			break;

		case 8:		// Character
			break;

		case 9:		// Door
			{
				Mesh mesh(level, i);
				roomMeshes.push_back(mesh);
			}
			break;

		case 10:	// Plushie
			break;

		case 11:	// Light
			break;

		case 12:	// Collectible
			break;

		case 13:	//Item

		default:
			break;
		}
	}


	Item item;
	item.SetItemType(1);
	item.SetPosition(glm::vec3(0, 1, 0));
	item.SetMaterialID(materials[1].GetMaterialID());
	items.push_back(item);
}

//=============================================================
//	Puzzle node initialization
//	Loads and positions all the puzzle nodes in the scene
//=============================================================
void Room::LoadPuzzleNode(std::vector<Material> materials)
{
	puzzleNode winNode;
	winNode.SetMaterialID(materials[0].GetMaterialID());
	winNode.SetPosition(glm::vec3(-5.0f, -0.4f, -9.0f));
	nodes.push_back(winNode);
}

