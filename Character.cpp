#include "Character.h"

Character::Character()
{
	playerMesh.CreateCubeData();

	glm::vec3 startPos = glm::vec3(0.0f, 0.0f, 0.0f);
	setPosition(startPos);
	boundingBoxSize = glm::vec3(0.5f, 0.5f, 0.5f);
	this->entityID = 1;
}

Character::~Character()
{

}

Primitive Character::getMeshData() const
{
	return playerMesh;
}

glm::vec3 Character::getPosition() const
{
	return playerMesh.getPosition();
}

glm::vec3 Character::getSize() const
{
	return boundingBoxSize;
}

void Character::setMaterialID(unsigned int id)
{
	playerMesh.setMaterial(id);
}

void Character::setPosition(glm::vec3 newPos)
{
	playerMesh.setPosition(newPos);
}

bool Character::CheckCollision(Entity collidingCube)
{
	struct AABB
	{
		glm::vec3 position;
		glm::vec3 size;
	};

	AABB thisBoundingBox;
	thisBoundingBox.position = getPosition();
	thisBoundingBox.size = boundingBoxSize;

	AABB collidingBoundingBox;
	collidingBoundingBox.position = collidingCube.GetPosition();
	collidingBoundingBox.size = collidingCube.GetBoundingBoxSize();

	glm::vec3 box1p1 = thisBoundingBox.position + thisBoundingBox.size;
	glm::vec3 box1p2 = thisBoundingBox.position - thisBoundingBox.size;
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

bool Character::CheckInBound(Entity collidingCube)
{
	struct AABB
	{
		glm::vec3 position;
		glm::vec3 size;
	};

	AABB collidingBoundingBox;
	collidingBoundingBox.position = collidingCube.getPosition();
	collidingBoundingBox.size = collidingCube.getSize();

	//=============================================================
	//	inBoundBox will only be used to check if there is any 
	//	object inside. If so, we can check for grab.
	//=============================================================
	AABB inBoundBox;
	inBoundBox.position = getPosition();
	inBoundBox.size = glm::vec3(0.7f, 0.5f, 0.7f);

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
glm::vec3 Character::Move(GLFWwindow* window, float dTime)
{
	float moveSpeed = 5.5f * dTime,
		moveX = 0.0f,
		moveY = 0.0f,
		moveZ = 0.0f;

	glm::vec3 newPos = playerMesh.getPosition();

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveX = moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveX = -moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveZ = moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveZ = -moveSpeed;

	return calcMovement(moveX, moveY, moveZ, playerMesh);
}

