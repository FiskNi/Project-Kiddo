#include "Character.h"

Character::Character() : RigidEntity(1)
{
	glm::vec3 startPos = glm::vec3(0.0f, 0.0f, 0.0f);
	SetPosition(startPos);
	SetGrounded(true);

	this->holdingObject = false;
	this->entityID = -1;
}

Character::~Character()
{

}

void Character::SetHoldingObject(bool holding)
{
	holdingObject = true;
}

void Character::SetEntityID(unsigned int id)
{
	this->entityID = id;
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
	collidingBoundingBox.size = collidingCube.GetBoundingBoxSize();

	//=============================================================
	//	inBoundBox will only be used to check if there is any 
	//	object inside. If so, we can check for grab.
	//=============================================================
	AABB inBoundBox;
	inBoundBox.position = GetPositionBB();

	// This is how large the use area around the character is
	const float bbOffset = 2.0f;
	inBoundBox.size = glm::vec3(GetBoundingBoxSize().x * bbOffset, 0.5f, GetBoundingBoxSize().y * bbOffset);

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
//	Moves this object based on keyboard input
//	Could be adapted into a keyboard callback
//=============================================================
glm::vec3 Character::Move(GLFWwindow* window)
{
	const float moveSpeed = 2.0f;
	const float maxSpeed = 5.0;
	float moveX = 0.0f;
	float moveY = 0.0f;
	float moveZ = 0.0f;
	glm::vec3 moveDir = glm::vec3(0.0f);


	if (glm::length(GetVelocity()) < maxSpeed)
	{
		// Player movement speed
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			moveX = moveSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			moveX = -moveSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			moveZ = moveSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			moveZ = -moveSpeed;
		}

		if (glm::length(GetVelocity()) > 0.5f)
		{
			glm::vec3 forwardZ(0.0, 0.0f, 1.0f);
			float cosRotation = glm::dot(forwardZ, glm::normalize(GetVelocity()));
			
			float rotation = acos(cosRotation);

			if(GetVelocityX() > 0)
				SetRotation(0.0f, rotation, 0.0f);
			else
				SetRotation(0.0f, -rotation, 0.0f);
		}

		moveDir = glm::vec3(moveX, moveY, moveZ);
		//moveDir = glm::normalize(moveDir);
		moveDir *= moveSpeed;
	}

	inputVector = moveDir;
	return glm::vec3(moveDir);
}