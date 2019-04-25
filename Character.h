#pragma once
#include "Headers.h"
#include "Primitive.h"
#include "RigidEntity.h"

class Character
	: public RigidEntity
{
private:
	//ID, playerID = 1, Movable box = 2.
	unsigned int entityID;
	bool jumpSquat = false;

public:
	Character();
	~Character();
	
	bool CheckInBound(Entity collidingCube);
	glm::vec3 Move(GLFWwindow *window);
};