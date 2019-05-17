#include "Room.h"



Room::Room(std::vector<Material> materials, Loader* aLoader, irrklang::ISoundEngine* audioEngine)
{
	LoadLights(aLoader);
	LoadEntities(materials, aLoader);
	isRoomCompleted = false;

	// Initialize camera (Default constructor)
	roomCamera = new Camera;

	firstCall = true;

	// Compiles all the mesh data in the room for the renderer
	CompileMeshData();

	this->audioEngine = audioEngine;
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

	// Simple animation loop
	for (int i = 0; i < roomMeshes.size(); i++)
	{
		roomMeshes[i].ForwardTime(deltaTime);
		if (roomMeshes[i].GetSkeleton().currentAnimTime > 5.0f)
			roomMeshes[i].SetTime(0.0f);
	}

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
	PlayerDoorCollision(playerCharacter);
	
	//PlayerItemCollision(playerCharacter);
	//PlayerCollectibleCollision(playerCharacter);

	// Game events
	// This is where link IDs will be added for each entity in the scene based on importer attributes

	// Convoluted super deluxe solution for handling no more than 2 pressure plates linking to the same bridge
	for (int i = 0; i < pressurePlates.size(); i++)
	{
		if (pressurePlates[i].isPressed())
		{
			for (int j = 0; j < bridges.size(); j++)
			{
				if ( !bridges[j].GetExtending() && bridges[j].CheckLinkID(pressurePlates[i].GetLinkID()) )
				{
					if (!bridges[j].GetExtending() && !bridges[j].GetExtended())
						// ADD SOUND PLAY
					bridges[j].Extend();			
				}
			}
		}
		else
		{
			bool matchingid = false;
			// Check for other pressureplates connecting to the same bridge
			for (int j = 0; j < pressurePlates.size(); j++)
			{
				if (pressurePlates[j].CheckLinkID(pressurePlates[i].GetLinkID()) && i != j)
				{				
					// Matching pressure plate found
					matchingid = true;
					if (!pressurePlates[j].isPressed())
					{
						// It's not pressed so it's safe to retract the bridge
						for (int p = 0; p < bridges.size(); p++)
						{
							if (bridges[p].CheckLinkID(pressurePlates[i].GetLinkID()) && bridges[p].CheckLinkID(pressurePlates[j].GetLinkID()))
							{
								bridges[p].Retract();
							}
						}
					}	
				}
			}
			if (!matchingid)
			{
				// No matching pressure plate so it's safe to retract bridge
				for (int p = 0; p < bridges.size(); p++)
				{
					if (bridges[p].CheckLinkID(pressurePlates[i].GetLinkID()))
					{
						bridges[p].Retract();
					}
				}
			}
		}
	}

	// Buttons linking to bridge
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].isPressed())
		{
			for (int j = 0; j < bridges.size(); j++)
			{
				if (bridges[j].CheckLinkID(buttons[i].GetLinkID()))
				{
					bridges[j].Extend();
				}
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
				// PLAY SOUND
			}
		}

		if (!pressurePlates[i].isPressed() && playerCharacter->CheckCollision(pressurePlates[i]) && pressurePlates[i].CheckInsideCollision(*playerCharacter))
		{
			pressurePlates[i].setPressed(true);
			// PLAY SOUND
		}
	}
}

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

void Room::PlayerDoorCollision(Character* playerCharacter)
{
	for (int i = 0; i < doors.size(); i++) {
		if (playerCharacter->CheckCollision(doors[i])) {
			isRoomCompleted = true;
		}
	}
}

//void Room::PlayerCollectibleCollision(Character * playerCharacter)
//{
//	for (int i = 0; i < collectibles.size(); i++) {
//		if (playerCharacter->CheckCollision(collectibles[i])) {
//			playerCharacter->PickUpCollectible(&collectibles[i]);
//			collectibles[i].SetPosition(glm::vec3(0, -30, 0));
//		}
//	}
//}
//
//void Room::PlayerItemCollision(Character* playerCharacter)
//{
//	for (int i = 0; i < items.size(); i++) {
//		if (playerCharacter->CheckCollision(items[i])) {
//			playerCharacter->PickUpItem(&items[i]);
//			items[i].SetPickedUp(true);
//		}
//	}
//}

//=============================================================
// This finds and sets the parent of a mesh by going through every different vector.
// This since the "meshes" vector is constantly being cleared.
// So it was the best way I could currently think of to make sure every parent is found.
// --MAKE SURE TO ADD NEW VECTORS IF WE ADD MORE OVER TIME.--
// Could potentially just go through a single vector practically identical to "meshes" vector except not cleared.
//=============================================================
bool Room::FindParent(Mesh * childMesh)
{

	//Meshgroups are most likely to be parents of meshes and are thus tested first.
	for (int j = 0; j < meshGroups.size(); j++)
	{
		if (meshGroups[j].GetName() == childMesh->GetMeshParentName())
		{
			childMesh->SetGroupParent(&meshGroups[j]);
			return true;
		}
	}

	for (int j = 0; j < roomMeshes.size(); j++)
	{
		if (roomMeshes[j].GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(&roomMeshes[j]);
			return true;
		}
	}

	for (int j = 0; j < statics.size(); j++)
	{
		if (statics[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(&roomMeshes[j]);
			return true;
		}
	}

	for (int j = 0; j < rigids.size(); j++)
	{
		if (rigids[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(&rigids[j].GetMeshData());
			return true;
		}
	}

	for (int j = 0; j < holders.size(); j++)
	{
		if (holders[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(&holders[j].GetMeshData());
			return true;
		}
	}

	for (int j = 0; j < buttons.size(); j++)
	{
		if (buttons[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(&buttons[j].GetMeshData());
			return true;
		}
	}

	for (int j = 0; j < pressurePlates.size(); j++)
	{
		if (pressurePlates[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(&pressurePlates[j].GetMeshData());
			return true;
		}
	}

	return false;
}

//=============================================================
// Same as above but for MeshGroups just in case.
//=============================================================
bool Room::FindParent(MeshGroupClass * childMeshGroup)
{

	for (int j = 0; j < roomMeshes.size(); j++)
	{
		if (roomMeshes[j].GetMeshName() == childMeshGroup->GetParentName())
		{
			childMeshGroup->SetMeshParent(&roomMeshes[j]);
			return true;
		}
	}

	for (int j = 0; j < statics.size(); j++)
	{
		if (statics[j].GetMeshData().GetMeshName() == childMeshGroup->GetParentName())
		{
			childMeshGroup->SetMeshParent(&roomMeshes[j]);
			return true;
		}
	}

	for (int j = 0; j < rigids.size(); j++)
	{
		if (rigids[j].GetMeshData().GetMeshName() == childMeshGroup->GetParentName())
		{
			childMeshGroup->SetMeshParent(&rigids[j].GetMeshData());
			return true;
		}
	}

	for (int j = 0; j < holders.size(); j++)
	{
		if (holders[j].GetMeshData().GetMeshName() == childMeshGroup->GetParentName())
		{
			childMeshGroup->SetMeshParent(&holders[j].GetMeshData());
			return true;
		}
	}

	for (int j = 0; j < buttons.size(); j++)
	{
		if (buttons[j].GetMeshData().GetMeshName() == childMeshGroup->GetParentName())
		{
			childMeshGroup->SetMeshParent(&buttons[j].GetMeshData());
			return true;
		}
	}

	for (int j = 0; j < pressurePlates.size(); j++)
	{
		if (pressurePlates[j].GetMeshData().GetMeshName() == childMeshGroup->GetParentName())
		{
			childMeshGroup->SetMeshParent(&pressurePlates[j].GetMeshData());
			return true;
		}
	}

	//Meshgroups are least likely to be parents of meshgroups and are thus tested last.
	for (int j = 0; j < meshGroups.size(); j++)
	{
		if (meshGroups[j].GetName() == childMeshGroup->GetParentName())
		{
			childMeshGroup->SetGroupParent(&meshGroups[j]);
			return true;
		}
	}

	return false;
}

void Room::SetAllParents()
{

	for (int i = 0; i < roomMeshes.size(); i++)
	{
		if (roomMeshes[i].GetIsChild() == true)
		{
			bool parentFound = FindParent(&roomMeshes[i]);
			if (parentFound == false)
			{
				std::cout << "CANNOT FIND 'roomMeshes' index:" + to_string(i) +
					" PARENT DESPITE BEING CHILD. SOMETHING WENT WRONG IN 'FindParent' FUNCTION!!" << endl;
				system("Pause");
			}
		}
	}

	for (int i = 0; i < statics.size(); i++)
	{
		if (statics[i].GetMeshData().GetIsChild() == true)
		{
			bool parentFound = FindParent(&statics[i].GetMeshData());
			if (parentFound == false)
			{
				cout << "CANNOT FIND 'statics' index: " + to_string(i) +
					" PARENT DESPITE BEING CHILD. SOMETHING WENT WRONG IN 'FindParent' FUNCTION!!" << endl;
				system("Pause");
			}
		}
	}

	for (int i = 0; i < rigids.size(); i++)
	{
		if (rigids[i].GetMeshData().GetIsChild() == true)
		{
			bool parentFound = FindParent(&rigids[i].GetMeshData());
			if (parentFound == false)
			{
				cout << "CANNOT FIND 'rigids' index: " + to_string(i) +
					" PARENT DESPITE BEING CHILD. SOMETHING WENT WRONG IN 'FindParent' FUNCTION!!" << endl;
				system("Pause");
			}
		}
	}

	for (int i = 0; i < holders.size(); i++)
	{
		if (holders[i].GetMeshData().GetIsChild() == true)
		{
			bool parentFound = FindParent(&holders[i].GetMeshData());
			if (parentFound == false)
			{
				cout << "CANNOT FIND 'holders' index: " + to_string(i) +
					" PARENT DESPITE BEING CHILD. SOMETHING WENT WRONG IN 'FindParent' FUNCTION!!" << endl;
				system("Pause");
			}
		}
	}

	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].GetMeshData().GetIsChild() == true)
		{
			bool parentFound = FindParent(&buttons[i].GetMeshData());
			if (parentFound == false)
			{
				cout << "CANNOT FIND 'buttons' index: " + to_string(i) +
					" PARENT DESPITE BEING CHILD. SOMETHING WENT WRONG IN 'FindParent' FUNCTION!!" << endl;
				system("Pause");
			}
		}
	}

	for (int i = 0; i < pressurePlates.size(); i++)
	{
		if (pressurePlates[i].GetMeshData().GetIsChild() == true)
		{
			bool parentFound = FindParent(&pressurePlates[i].GetMeshData());
			if (parentFound == false)
			{
				cout << "CANNOT FIND 'preassurePlates' index: " + to_string(i) +
					" PARENT DESPITE BEING CHILD. SOMETHING WENT WRONG IN 'FindParent' FUNCTION!!" << endl;
				system("Pause");
			}
		}
	}

	for (int i = 0; i < meshGroups.size(); i++)
	{
		if (meshGroups[i].GetIsChild() == true)
		{
			bool parentFound = FindParent(&meshGroups[i]);
			if (parentFound == false)
			{
				std::cout << "CANNOT FIND 'meshGroups' index: " + to_string(i) +
					" PARENT DESPITE BEING CHILD. SOMETHING WENT WRONG IN 'FindParent' FUNCTION!!" << endl;
				system("Pause");
			}
		}
	}
}

std::vector <float> Room::GetParentOffset(Mesh * childMesh)
{
	if (childMesh->GetIsChild() == true)
	{
		if (childMesh->GetParentType() == 1)
		{
			//Initalize vector of size 9 with all 0s
			std::vector <float> offset(9,0);
			//Size can't be 0
			offset[6] = 1;
			offset[7] = 1;
			offset[8] = 1;
			//Get grandparent and grand-grand-parents offset and so on
			if (childMesh->GetMeshParent()->GetIsChild() == true)
			{
				offset = GetParentOffset(childMesh->GetMeshParent());
			}

			offset[0] += childMesh->GetMeshParent()->GetPosition().x;
			offset[1] += childMesh->GetMeshParent()->GetPosition().y;
			offset[2] += childMesh->GetMeshParent()->GetPosition().z;
			offset[3] += childMesh->GetMeshParent()->GetRotation().x;
			offset[4] += childMesh->GetMeshParent()->GetRotation().y;
			offset[5] += childMesh->GetMeshParent()->GetRotation().z;
			offset[6] *= childMesh->GetMeshParent()->GetScale().x;
			offset[7] *= childMesh->GetMeshParent()->GetScale().y;
			offset[8] *= childMesh->GetMeshParent()->GetScale().z;

			return offset;
		}
		else if (childMesh->GetParentType() == 0)
		{
			std::vector <float> offset(9, 0);
			//Size can't be 0
			offset[6] = 1;
			offset[7] = 1;
			offset[8] = 1;
			//Get grandparent and grand-grand-parents offset and so on
			if (childMesh->GetGroupParent()->GetIsChild() == true)
			{
				//Note we call the other version of the function here.
				offset = GetParentOffset(childMesh->GetGroupParent());
			}

			offset[0] += childMesh->GetGroupParent()->GetGroupPosition().x;
			offset[1] += childMesh->GetGroupParent()->GetGroupPosition().y;
			offset[2] += childMesh->GetGroupParent()->GetGroupPosition().z;
			offset[3] += childMesh->GetGroupParent()->GetGroupRotation().x;
			offset[4] += childMesh->GetGroupParent()->GetGroupRotation().y;
			offset[5] += childMesh->GetGroupParent()->GetGroupRotation().z;
			offset[6] *= childMesh->GetGroupParent()->GetGroupScale().x;
			offset[7] *= childMesh->GetGroupParent()->GetGroupScale().y;
			offset[8] *= childMesh->GetGroupParent()->GetGroupScale().z;

			return offset;
		}
	}

	std::cout << "GetParentOffset (mesh) INPUT IS NOT CHILD. PLEASE ONLY INPUT CHILD MESHES." << endl;

	return std::vector<float>(1,-1);
}

std::vector <float> Room::GetParentOffset(MeshGroupClass * childGroup)
{
	if (childGroup->GetIsChild() == true)
	{
		if (childGroup->GetParentType() == 1)
		{

			std::vector <float> offset(9, 0);
			//Size can't be 0
			offset[6] = 1;
			offset[7] = 1;
			offset[8] = 1;
			//Get grandparent and grand-grand-parents offset and so on
			if (childGroup->GetMeshParent()->GetIsChild() == true)
			{
				offset = GetParentOffset(childGroup->GetMeshParent());
			}

			offset[0] += childGroup->GetMeshParent()->GetPosition().x;
			offset[1] += childGroup->GetMeshParent()->GetPosition().y;
			offset[2] += childGroup->GetMeshParent()->GetPosition().z;
			offset[3] += childGroup->GetMeshParent()->GetRotation().x;
			offset[4] += childGroup->GetMeshParent()->GetRotation().y;
			offset[5] += childGroup->GetMeshParent()->GetRotation().z;
			offset[6] *= childGroup->GetMeshParent()->GetScale().x;
			offset[7] *= childGroup->GetMeshParent()->GetScale().y;
			offset[8] *= childGroup->GetMeshParent()->GetScale().z;

			//Special only wanna do this for groups since they're not updated outside
			glm::vec3 temp = childGroup->GetGroupPosition() * glm::vec3(offset[6], offset[7], offset[8]) + glm::vec3(offset[0], offset[1], offset[2]);

			offset[0] = temp[0];
			offset[1] = temp[1];
			offset[2] = temp[2];


			return offset;
		}
		else if (childGroup->GetParentType() == 0)
		{
			std::vector <float> offset(9, 0);
			//Size can't be 0
			offset[6] = 1;
			offset[7] = 1;
			offset[8] = 1;
			//Get grandparent and grand-grand-parents offset and so on
			if (childGroup->GetGroupParent()->GetIsChild() == true)
			{
				//I'm going to have to make a second overloaded function... Fuck this shit I'm OUT!
				offset = GetParentOffset(childGroup->GetGroupParent());
			}

			offset[0] += childGroup->GetGroupParent()->GetGroupPosition().x;
			offset[1] += childGroup->GetGroupParent()->GetGroupPosition().y;
			offset[2] += childGroup->GetGroupParent()->GetGroupPosition().z;
			offset[3] += childGroup->GetGroupParent()->GetGroupRotation().x;
			offset[4] += childGroup->GetGroupParent()->GetGroupRotation().y;
			offset[5] += childGroup->GetGroupParent()->GetGroupRotation().z;
			offset[6] *= childGroup->GetGroupParent()->GetGroupScale().x;
			offset[7] *= childGroup->GetGroupParent()->GetGroupScale().y;
			offset[8] *= childGroup->GetGroupParent()->GetGroupScale().z;

			return offset;
		}
	}

	std::cout << "GetParentOffset (group) INPUT IS NOT CHILD. PLEASE ONLY INPUT CHILD GROUPS." << endl;

	return std::vector<float>(1, -1);
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
	//if (isRoomCompleted != true) {
	//	for (int i = 0; i < rigids.size(); i++)
	//	{
	//		if (nodes[0].CheckCollision(rigids[i]))
	//		{
	//			for (int j = 0; j < rigids.size(); ++j)
	//			{
	//				cout << "Solved" << endl;
	//				isRoomCompleted = true;
	//			}
	//		}
	//	}
	//}
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

//void Room::Upgrade(Character* playerCharacter)
//{
//	Item* temp = playerCharacter->GetCurrentItem();
//	if (temp != nullptr) {
//		for (int i = 0; i < rigids.size(); i++) {
//			if (playerCharacter->CheckInBound(rigids[i])) {
//				rigids[i].SetBoxType(playerCharacter->GetCurrentItem()->GetItemType());
//				if (rigids[i].GetBoxType() == 1) {
//					std::cout << "box upgraded" << std::endl;
//				}
//				//playerCharacter->ItemUsed();
//
//			}
//		}
//	}
//
//}

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

	//for (int i = 0; i < nodes.size(); i++)
	//{
	//	meshes.push_back(nodes[i].GetMeshData());
	//}

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

		meshes.push_back(items[i].GetMeshData());
	}
	for (int i = 0; i < doors.size(); i++) {
		meshes.push_back(doors[i].GetMeshData());
	}
	for (int i = 0; i < collectibles.size(); i++) {
		meshes.push_back(collectibles[i].GetMeshData());
	}

	//Applying all parent data on the child mesh

	if (firstCall == true)
	{
		firstCall = false;

		for (int i = 0; i < meshes.size(); i++)
		{
			if (meshes[i].GetIsChild() == true)
			{
				std::vector <float> temp = GetParentOffset(&meshes[i]);
				glm::vec3 posVec = glm::vec3(temp[0], temp[1], temp[2]);
				glm::vec3 rotVec = glm::vec3(temp[3], temp[4], temp[5]);
				glm::vec3 sizeVec = glm::vec3(temp[6], temp[7], temp[8]);

				//If a child object has a position in maya it is affected by the size vectors of its parents.
				meshes[i].SetPosition(meshes[i].GetPosition()*sizeVec + glm::vec3(temp[0], temp[1], temp[2]));
				meshes[i].SetScale(meshes[i].GetScale()*sizeVec);
			}
		}
	}
	//I imagine updates after could go something like 
	//origPosNew = "origGetPosition"*sizeVec + posVec
	//NewPos = curPos + origPosNew - OrigPosOld;
	//Alternatively we could just straight up add the new movement but that assumes we never change the object size
	//Ooooooor we could just save offset vector alt size vec. We just remove it and add the new one.


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
				//Mesh mesh(level, i);
				//roomMeshes.push_back(mesh);
				Door door(level, i, matID);
				//door.SetPosition(glm::vec3(-40, 0.5, 5));
				doors.push_back(door);
			}
			break;

		case 10:	// Plushie
			break;

		case 11:	// Light
			break;

		case 12:	// Collectible
		{

			Collectible coll;
			coll.SetMaterialID(matID);
			collectibles.push_back(coll);
		}
			break;

		case 13:	//Item
		{
			Item item;
			//Need to look over how to import itemtype :)
			//item.SetItemType()
			item.SetMaterialID(matID);
			items.push_back(item);
		}

		default:
			break;
		}
	}

	for (int i = 0; i < level->GetMeshGroupCount(); i++)
	{
		MeshGroupClass group(level, i);
		meshGroups.push_back(group);
	}


	//Finding and setting parents so that things can be moved properly later.
	SetAllParents();
}
