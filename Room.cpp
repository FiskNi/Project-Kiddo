#include "Room.h"



Room::Room(Loader* aLoader, irrklang::ISoundEngine* audioEngine)
{

	firstCall = true;
	meshAmount = 0;

	LoadLights(aLoader);
	LoadEntities(aLoader);
	isRoomCompleted = false;

	// Initialize camera (Default constructor)
	roomCamera = new Camera;
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
		if (roomMeshes[i].GetSkeleton().currentAnimTime > 2.0f)
			roomMeshes[i].SetTime(0);
		if (roomMeshes[i].GetSkeleton().currentAnimTime < 0.0f)
			roomMeshes[i].SetTime(0);

		if (roomMeshes[i].GetSkeleton().currentAnimTime >= 1.1f)
			roomMeshes[i].SetPlayingBackwards(true);
		if (roomMeshes[i].GetSkeleton().currentAnimTime <= 0.0f)
			roomMeshes[i].SetPlayingBackwards(false);

		if (!roomMeshes[i].GetSkeleton().playingBackwards)
			roomMeshes[i].ForwardTime(deltaTime);
		else if(roomMeshes[i].GetSkeleton().playingBackwards)
			roomMeshes[i].BackwardTime(deltaTime);
	}

	// Reset collisions
	playerCharacter->SetColliding(false);
	playerCharacter->SetHoldingObject(false);
	for (int i = 0; i < rigids.size(); i++)
	{
		rigids[i].SetColliding(false);
		rigids[i].SetHeld(false);
	}


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
	playerCharacter->SetEntityID(inBoundCheck(*playerCharacter));	

	if (playerCharacter->GetEntityID() >= 0)
	{
		if (playerCharacter->CheckInBound(rigids[playerCharacter->GetEntityID()]))
		{
			if (glfwGetKey(renderWindow, GLFW_KEY_SPACE) == GLFW_PRESS ||
				glfwGetKey(renderWindow, GLFW_KEY_L) == GLFW_PRESS ||
				glfwGetKey(renderWindow, GLFW_KEY_E) == GLFW_PRESS)
			{
				
				rigids[playerCharacter->GetEntityID()].AddVelocity(playerCharacter->GetInputVector());
				rigids[playerCharacter->GetEntityID()].SetHeld(true);
				playerCharacter->SetHoldingObject(true);
			
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

void Room::PlayerCollectibleCollision(Character * playerCharacter)
{
	for (int i = 0; i < collectibles.size(); i++) {
		if (playerCharacter->CheckCollision(collectibles[i])) {
			playerCharacter->PickUpCollectible(&collectibles[i]);
			collectibles[i].SetPosition(glm::vec3(0, -30, 0));
		}
	}
}

void Room::PlayerItemCollision(Character* playerCharacter)
{
	for (int i = 0; i < items.size(); i++) {
		if (playerCharacter->CheckCollision(items[i])) {
			/*playerCharacter->PickUpItem(&items[i]);
			items[i].SetPickedUp(true);*/
		}
	}
}

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
			childMesh->SetMeshParent(statics[j].GetMeshDataPointer());
			return true;
		}
	}

	for (int j = 0; j < rigids.size(); j++)
	{
		if (rigids[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(rigids[j].GetMeshDataPointer());
			return true;
		}
	}

	for (int j = 0; j < holders.size(); j++)
	{
		if (holders[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(holders[j].GetMeshDataPointer());
			return true;
		}
	}

	for (int j = 0; j < buttons.size(); j++)
	{
		if (buttons[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(buttons[j].GetMeshDataPointer());
			return true;
		}
	}

	for (int j = 0; j < pressurePlates.size(); j++)
	{
		if (pressurePlates[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(pressurePlates[j].GetMeshDataPointer());
			return true;
		}
	}

	for (int j = 0; j < doors.size(); j++)
	{
		if (doors[j].GetMeshData().GetMeshName() == childMesh->GetMeshParentName())
		{
			childMesh->SetMeshParent(doors[j].GetMeshDataPointer());
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

//=============================================================
// This finds and sets the parents of all meshes by calling FindParent for every mesh.
// NEW VECTORS SHOULD BE ADDED OVER TIME
//=============================================================
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
			bool parentFound = FindParent(statics[i].GetMeshDataPointer());
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
			bool parentFound = FindParent(rigids[i].GetMeshDataPointer());
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
			bool parentFound = FindParent(holders[i].GetMeshDataPointer());
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
			bool parentFound = FindParent(buttons[i].GetMeshDataPointer());
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
			bool parentFound = FindParent(pressurePlates[i].GetMeshDataPointer());
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

	for (int i = 0; i < doors.size(); i++)
	{
		if (doors[i].GetMeshData().GetIsChild() == true)
		{
			bool parentFound = FindParent(doors[i].GetMeshDataPointer());
			if (parentFound == false)
			{
				cout << "CANNOT FIND 'doors' index: " + to_string(i) +
					" PARENT DESPITE BEING CHILD. SOMETHING WENT WRONG IN 'FindParent' FUNCTION!!" << endl;
				system("Pause");
			}
		}
	}
}

std::vector<float> Room::GetParentOffset(Mesh * childMesh)
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

std::vector<float> Room::GetParentOffset(MeshGroupClass * childGroup)
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

glm::vec3 Room::updateChild(Mesh * meshPtr)
{
	glm::vec3 returnVec = glm::vec3(0,0,0);
	if (firstCall == false)
	{
		if (meshPtr->GetParentType() == 1)
		{
			if (meshPtr->GetMeshParent()->GetPosition() != meshPtr->GetParentPosOffset())
			{
				returnVec = (meshPtr->GetMeshParent()->GetPosition() - meshPtr->GetParentPosOffset());
				meshPtr->SetParentPosOffset(meshPtr->GetMeshParent()->GetPosition());
			}

		}
		else if (meshPtr->GetParentType() == 0)
		{
			if (meshPtr->GetGroupParent()->GetGroupPosition() != meshPtr->GetParentPosOffset())
			{
				returnVec = (meshPtr->GetGroupParent()->GetGroupPosition() - meshPtr->GetParentPosOffset());
				meshPtr->SetParentPosOffset(meshPtr->GetGroupParent()->GetGroupPosition());
			}
		}
	}
	else
	{
		if (meshPtr->GetParentType() == 1)
		{
			//The setting of position won't bloody stick
			meshPtr->SetParentPosOffset(meshPtr->GetMeshParent()->GetPosition());
		}
		else if (meshPtr->GetParentType() == 0)
		{
			//Issue: the groups aren't actually moved >~<
			meshPtr->SetParentPosOffset(meshPtr->GetGroupParent()->GetGroupPosition());
		}
	}

	return returnVec;
}

glm::vec3 Room::updateChild(MeshGroupClass * meshPtr)
{
	glm::vec3 returnVec = glm::vec3(0, 0, 0);
	if (firstCall == false)
	{
		if (meshPtr->GetParentType() == 1)
		{
			if (meshPtr->GetMeshParent()->GetPosition() != meshPtr->GetParentPosOffset())
			{
				returnVec = (meshPtr->GetMeshParent()->GetPosition() - meshPtr->GetParentPosOffset());
				meshPtr->SetParentPosOffset(meshPtr->GetMeshParent()->GetPosition());
			}

		}
		else if (meshPtr->GetParentType() == 0)
		{
			if (meshPtr->GetGroupParent()->GetGroupPosition() != meshPtr->GetParentPosOffset())
			{
				returnVec = (meshPtr->GetGroupParent()->GetGroupPosition() - meshPtr->GetParentPosOffset());
				meshPtr->SetParentPosOffset(meshPtr->GetGroupParent()->GetGroupPosition());
			}
		}
	}
	else
	{
		if (meshPtr->GetParentType() == 1)
		{
			//The setting of position won't bloody stick
			meshPtr->SetParentPosOffset(meshPtr->GetMeshParent()->GetPosition());
		}
		else if (meshPtr->GetParentType() == 0)
		{
			//Issue: the groups aren't actually moved >~<
			meshPtr->SetParentPosOffset(meshPtr->GetGroupParent()->GetGroupPosition());
		}
	}

	return returnVec;
}

void Room::updateChildren()
{
	for (int i = 0; i < roomMeshes.size(); i++)
	{
		if (roomMeshes[i].GetIsChild() == true)
		{
			roomMeshes[i].SetPosition(roomMeshes[i].GetPosition() + updateChild(&roomMeshes[i]));
		}
	}

	for (int i = 0; i < statics.size(); i++)
	{
		if (statics[i].GetMeshData().GetIsChild() == true)
		{
			Mesh * meshPtr = statics[i].GetMeshDataPointer();
			glm::vec3 change = updateChild(meshPtr);
			statics[i].SetPosition(statics[i].GetPosition() + glm::vec3(change[0], 0, change[2]));
		}
	}

	for (int i = 0; i < rigids.size(); i++)
	{
		if (rigids[i].GetMeshData().GetIsChild() == true)
		{
			Mesh * meshPtr = rigids[i].GetMeshDataPointer();
			/*rigids[i].SetPosition(rigids[i].GetPosition() + updateChild(meshPtr));*/
			rigids[i].AddVelocity(updateChild(meshPtr)*glm::vec3(25,25,25));
		}
	}

	for (int i = 0; i < holders.size(); i++)
	{
		if (holders[i].GetMeshData().GetIsChild() == true)
		{
			Mesh * meshPtr = holders[i].GetMeshDataPointer();
			holders[i].SetPosition(holders[i].GetPosition() + updateChild(meshPtr));
		}
	}

	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].GetMeshData().GetIsChild() == true)
		{
			Mesh * meshPtr = buttons[i].GetMeshDataPointer();
			buttons[i].SetPosition(buttons[i].GetPosition() + updateChild(meshPtr));
		}
	}

	for (int i = 0; i < pressurePlates.size(); i++)
	{
		if (pressurePlates[i].GetMeshData().GetIsChild() == true)
		{
			Mesh * meshPtr = pressurePlates[i].GetMeshDataPointer();
			pressurePlates[i].SetPosition(pressurePlates[i].GetPosition() + updateChild(meshPtr));
		}
	}

	for (int i = 0; i < meshGroups.size(); i++)
	{
		if (meshGroups[i].GetIsChild() == true)
		{
			meshGroups[i].SetPosition(meshGroups[i].GetGroupPosition() + updateChild(&meshGroups[i]));
		}
	}

	for (int i = 0; i < doors.size(); i++)
	{
		if (doors[i].GetMeshData().GetIsChild() == true)
		{
			Mesh * meshPtr = doors[i].GetMeshDataPointer();
			doors[i].SetPosition(doors[i].GetPosition() + updateChild(meshPtr));
		}
	}
}



//=============================================================
//	Checks all rigid collisions with the ground, includes the player.
//	This loops trough all the rigids and all the statics in the scene.
//	Afterwards does a collision check and applies the highest ground level found.
//	The actual action on collison happens in the rigid entity class.
//=============================================================
void Room::RigidGroundCollision(Character* playerCharacter)
{
	const float maxDiff = 0.7f; // Max ground height difference

	 //Rigid entites ground collision
	for (int i = 0; i < rigids.size(); i++)
	{
		// Recheck grounded state, assume it's not grounded
		rigids[i].SetGrounded(false);

		// Variable to find the highest ground level
		float ground = -999.0f;

		// All the statics
		for (int j = 0; j < statics.size(); ++j)
		{
			if (rigids[i].CheckCollision(statics[j]))
			{
				// If ground is close enough
				if (abs(rigids[i].GetHitboxBottom() - statics[j].GetHitboxTop()) < maxDiff)
				{
					if (statics[j].GetHitboxTop() > ground)
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
					if (statics[j].GetHitboxTop() > ground)
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
	float ground = -999.0f;
	for (int j = 0; j < statics.size(); ++j)
	{
		if (playerCharacter->CheckCollision(statics[j]))
		{
			// If ground is close enough
			if (abs(playerCharacter->GetHitboxBottom() - statics[j].GetHitboxTop()) < maxDiff)
			{
				if (statics[j].GetHitboxTop() > ground)
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
				if (statics[j].GetHitboxTop() > ground)
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
	//===================================
	//=========OLD PUSHABLE CODE=========
	//===================================
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


	for (int i = 0; i < rigids.size(); ++i)
	{
		if (playerCharacter->CheckCollision(rigids[i]))
		{
			if (abs(rigids[i].GetHitboxTop() - playerCharacter->GetHitboxBottom()) >= 0.5f)
			{
				glm::vec3 pushDir = rigids[i].GetPosition() - playerCharacter->GetPosition();

				pushDir = normalize(pushDir);

				pushDir.y = 0.0f;
				pushDir *= 3.0f;

				//playerCharacter->SetVelocity(-pushDir);
				playerCharacter->SetPosition(playerCharacter->GetSavedPos());
				playerCharacter->SetColliding(true);
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

					//pushDir.y = 0.0f;
					//pushDir *= 3.0f;

					//// Lock to 1 axis
					//if (abs(pushDir.x) >= abs(pushDir.z))
					//	pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
					//else
					//	pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
					//pushDir *= 2.0f;

					//rigids[i].AddVelocity(pushDir);

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

				//playerCharacter->SetVelocity(-pushDir);
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

				//playerCharacter->SetVelocity(-pushDir);
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

void Room::Upgrade(Character* playerCharacter)
{
	//Item* temp = playerCharacter->GetCurrentItem();
	//if (temp != nullptr) {
	//	for (int i = 0; i < rigids.size(); i++) {
	//		if (playerCharacter->CheckInBound(rigids[i])) {
	//			rigids[i].SetBoxType(playerCharacter->GetCurrentItem()->GetItemType());
	//			if (rigids[i].GetBoxType() == 1) {
	//				std::cout << "box upgraded" << std::endl;
	//			}
	//			//playerCharacter->ItemUsed();

	//		}
	//	}
	//}
}

//=============================================================
//	Compiles mesh data for the renderer
//=============================================================
void Room::CompileMeshData()
{

	int j = 0;
	for (int i = 0; i < roomMeshes.size(); i++)
	{
		meshes[j] = roomMeshes[i];
		j++;
	}

	for (int i = 0; i < rigids.size(); i++)
	{
		meshes[j] = rigids[i].GetMeshData();
		j++;
	}

	for (int i = 0; i < statics.size(); i++)
	{
		meshes[j] = statics[i].GetMeshData();
		j++;
	}

	for (int i = 0; i < bridges.size(); i++)
	{
		meshes[j] = bridges[i].GetMeshData();
		j++;
	}

	for (int i = 0; i < pressurePlates.size(); i++) 
	{
		meshes[j] = pressurePlates[i].GetMeshData();
		j++;
	}

	for (int i = 0; i < buttons.size(); i++) 
	{
		meshes[j] = buttons[i].GetMeshData();
		j++;
	}

	for (int i = 0; i < holders.size(); i++)
	{
		meshes[j] = holders[i].GetMeshData();
		j++;
		meshes[j] = holders[i].GetHolderMeshData();
		j++;
	}

	for (int i = 0; i < items.size(); i++) {

		meshes[j] = items[i].GetMeshData();
		j++;
	}

	for (int i = 0; i < doors.size(); i++) {
		meshes[j] = doors[i].GetMeshData();
		j++;
	}

	for (int i = 0; i < collectibles.size(); i++) {
		meshes[j] = collectibles[i].GetMeshData();
		j++;
	}
	
	//Applying all parent data on the child mesh
	updateChildren();
	firstCall = false;
}

//=============================================================
//	Light initialization
//	Loads and positions all the lights in the scene
//=============================================================
void Room::LoadLights(Loader* inLoader)
{
	Light light(0.0f, 0.0f, 0.0f, 0.0f, 300, 4);

	light.SetDiffuse(glm::vec3(0.0f, 0.0, 0.0f));
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
		pointLights[i].setPower(6.0f);
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
		dirLights[i].SetDiffuse(color);
		dirLights[i].SetStrength(0.4f);
	}

}

//=============================================================
//	Entity initialization
//	Loads and positions all the entities in the scene
//=============================================================
void Room::LoadEntities(Loader* level)
{

	//==========
	// Entity loading will be changed to take in custom attributes and base    what is loaded into the room on these
	// Will need to move the Loader up from this location to properly take in  materials as well
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
				meshAmount++;
			}
			break;
		case 1:		// Mesh
			{
				Mesh mesh(level, i);
				roomMeshes.push_back(mesh);
				meshAmount++;
			}
			break;

		case 2:		// Static
			{
				StaticEntity levelEntity(level, i, matID, true);
				statics.push_back(levelEntity);
				meshAmount++;
			}
			break;

		case 3:		// Rigid
			{
				RigidEntity cubeEntity(level, i, matID, true);
				cubeEntity.OffsetPositionY(3.0f);
				cubeEntity.SetStartPosition(cubeEntity.GetPosition());
				rigids.push_back(cubeEntity);
				meshAmount++;
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
				meshAmount++;
			}
			break;

		case 5:		// Box Holder
			{
				boxHolder boxHolderEntity(level, i, matID, matID, true);
				boxHolderEntity.puntBox();
				this->holders.push_back(boxHolderEntity);
				meshAmount++;
				meshAmount++;
			}
			break;

		case 6:		// Button
			{
				Button button(level, i, matID, true);
				button.SetLink(level->GetMesh(i).link);
				button.SetMaterialID(matID);
				button.scaleBB(2);
				buttons.push_back(button);
				meshAmount++;
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
				meshAmount++;
			}
			break;

		case 8:		// Character
			break;

		case 9:		// Door
			{
				Door door(level, i, matID);
				doors.push_back(door);
				meshAmount++;
			}
			break;

		case 10:	// Plushie
			break;

		case 11:	// Light
			break;

		case 12:	// Collectible
		{
			/*Collectible coll;
			coll.SetMaterialID(matID);
			coll.SetIndex(level->GetCollectIndex(i));
			collectibles.push_back(coll);
			meshAmount++;*/
		}
			break;

		case 13:	//Item
		{
			//Item item;
			////Need to look over how to import itemtype :)
			////item.SetItemType()
			//item.SetMaterialID(matID);
			//items.push_back(item);
			//meshAmount++;
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
	
	// Allocate memory for the mesh vector
	meshes.resize(meshAmount + 1);
	//Finding and setting parents so that things can be moved properly later.
	SetAllParents();
}
