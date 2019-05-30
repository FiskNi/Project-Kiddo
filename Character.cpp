#include "Character.h"

Character::Character() : RigidEntity(1)
{
	for (int i = 0; i < itemCap; i++) 
	{
		items.push_back(Item());
	}
	
	for (int i = 0; i < COLLECTEDCAP; i++) {

		collected.push_back(Collectible());
	}

	holdingObject = false;
	entityID = -1;
	inputVector = glm::vec3(0.0f);
	respawnPos = glm::vec3(0.0f);
}

Character::Character(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen) : RigidEntity(inLoader, index, matID, frozen)
{
	for (int i = 0; i < itemCap; i++)
	{
		items.push_back(Item());
	}

	for (int i = 0; i < COLLECTEDCAP; i++) {

		collected.push_back(Collectible());
	}

	holdingObject = false;
	entityID = -1;
	inputVector = glm::vec3(0.0f);
	respawnPos = glm::vec3(0.0f);
}

Character::Character(Loader* inLoader, unsigned int index, unsigned int matID) : RigidEntity(inLoader, index, matID)
{
	for (int i = 0; i < itemCap; i++)
	{
		items.push_back(Item());
	}

	for (int i = 0; i < COLLECTEDCAP; i++) 
	{
		collected.push_back(Collectible());
	}

	holdingObject = false;
	entityID = -1;
	inputVector = glm::vec3(0.0f);
	respawnPos = glm::vec3(0.0f);
}

Character::~Character()
{

}

void Character::SetHoldingObject(bool holding)
{
	holdingObject = holding;
}

void Character::SetEntityID(unsigned int id)
{
	entityID = id;
}

bool Character::CheckInBound(Entity collidingCube)
{
	struct AABB
	{
		glm::vec3 position;
		glm::vec3 size;
	};

	AABB collidingBoundingBox;
	collidingBoundingBox.position = collidingCube.GetPositionBB();
	collidingBoundingBox.size = collidingCube.GetHitboxSize();

	//=============================================================
	//	inBoundBox will only be used to check if there is any 
	//	object inside. If so, we can check for grab.
	//=============================================================
	AABB inBoundBox;
	inBoundBox.position = GetPositionBB();

	// This is how large the use area around the character is
	const float bbOffset = 2.0f;
	inBoundBox.size = glm::vec3(GetHitboxSize().x * bbOffset, GetHitboxSize().y, GetHitboxSize().z * bbOffset);

	//=============================================================
	//	The collision check here is mostly identical to 
	//	CollisionCheck function. Only difference is the fact is 
	//	that we check with the new inBoundBox.
	//=============================================================
	
	glm::vec3 box1p1 = inBoundBox.position + inBoundBox.size;
	glm::vec3 box1p2 = inBoundBox.position - inBoundBox.size;
	glm::vec3 box1min = glm::vec3(fminf(box1p1.x, box1p2.x), fminf(box1p1.y, box1p2.y), fminf(box1p1.z, box1p2.z));
	glm::vec3 box1max = glm::vec3(fmaxf(box1p1.x, box1p2.x), fmaxf(box1p1.y, box1p2.y), fmaxf(box1p1.z, box1p2.z));

	glm::vec3 box2p1 = collidingBoundingBox.position + collidingBoundingBox.size;
	glm::vec3 box2p2 = collidingBoundingBox.position - collidingBoundingBox.size;
	glm::vec3 box2min = glm::vec3(fminf(box2p1.x, box2p2.x), fminf(box2p1.y, box2p2.y), fminf(box2p1.z, box2p2.z));
	glm::vec3 box2max = glm::vec3(fmaxf(box2p1.x, box2p2.x), fmaxf(box2p1.y, box2p2.y), fmaxf(box2p1.z, box2p2.z));

	if ((box1min.x <= box2max.x && box1max.x >= box2min.x) &&
		(box1min.y <= box2max.y && box1max.y >= box2min.y) &&
		(box1min.z <= box2max.z && box1max.z >= box2min.z))
	{
		return true;
	}

	return false;
}

//=============================================================
//	Calculates a movement vector based on input
//	Could be adapted into a keyboard callback
//=============================================================
void Character::Move(GLFWwindow* window)
{
	// Player movement speed
	const float moveSpeed = 2.0f;
	const float maxSpeed = 4.0;
	float moveX = 0.0f;
	float moveY = 0.0f;
	float moveZ = 0.0f;
	glm::vec3 moveDir = glm::vec3(0.0f);


	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_RELEASE && RigidEntity::IsGrounded())
	{
       /*	jumpSquat = true;
		moveY = 1.0f;
		SetGrounded(false);*/
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (jumpSquat && RigidEntity::IsGrounded()) 
		{
			moveX = -moveSpeed / 5; 
			moveY = moveSpeed;

			SetGrounded(false);
		}
		else if (RigidEntity::IsGrounded())
		{
			moveX += -1.0f;
			lastDir = 1;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if(jumpSquat && RigidEntity::IsGrounded())
		{
			moveX = moveSpeed / 5;
			moveY = moveSpeed;
			SetGrounded(false);
		}
		else if (RigidEntity::IsGrounded())
		{
			moveX += 1.0f;
			lastDir = 2;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (jumpSquat && RigidEntity::IsGrounded()) 
		{
			moveZ = -moveSpeed / 5;
			moveY = moveSpeed;
			SetGrounded(false);
		}
		else if (RigidEntity::IsGrounded())
		{
			moveZ += -1.0f;
			lastDir = 3;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (jumpSquat && RigidEntity::IsGrounded())
		{
			moveZ = moveSpeed / 5;
			moveY = moveSpeed;
			SetGrounded(false);
		}
		else if (RigidEntity::IsGrounded())
		{
			moveZ += 1.0f;
			lastDir = 4;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		jumpSquat = false;

	if (glm::length(GetVelocity()) > 0.1f && !holdingObject)
	{
		if (!holdingObject) 
		{
			glm::vec3 forwardZ(0.0f, 0.0f, 1.0f);
			float cosRotation = glm::dot(forwardZ, glm::normalize(GetVelocity()));
			float rotation = acos(cosRotation);

			if (GetVelocityX() > 0)
				rotation;
			else
				rotation = -rotation;

			glm::quat qRotation = glm::quat(glm::vec3(0.0f, rotation, 0.0f));
			SetRotation(qRotation);
		}
	}

	moveDir = glm::vec3(moveX, moveY, moveZ);
	if (glm::length(moveDir) >= 0.1f)
		moveDir = glm::normalize(moveDir);

	if (holdingObject)
		moveDir *= moveSpeed / 1.5;
	else
		moveDir *= moveSpeed;
		
	inputVector = moveDir;
}

std::vector<Collectible>& Character::GetCollectedCollectibles()
{
	return collected;
}

//void Character::PickUpItem(Item * item)
//{
//	//if (nrOf == cap) {
//	//	return;
//	//}
//	//else {
//	//	for (int i = 0; i < cap; i++) {
//	//		if (items[i]->GetItemType() == NONE) {
//	//			items[i]->SetItemType(item->GetItemType());
//	//			nrOf++;
//	//			std::cout << "Picked up item" << std::endl;
//	//			break;
//	//		}
//	//	}
//	//}
//}

void Character::PickUpCollectible(Collectible* coll)
{
	if (!collected[coll->GetIndex()].GetCollected()) {
		collected[coll->GetIndex()].SetCollected(true);
	}
}

//Item * Character::Upgrade()
//{
//	if (items[currentItem] == nullptr) {
//		return nullptr;
//	}
//	else {
//		return items[currentItem];
//	}
//
//}


