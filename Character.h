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
	bool holdingObject;

public:
	Character();
	~Character();
	
	void SetHoldingObject(bool holding);
	void SetEntityID(unsigned int id);

	bool CheckInBound(Entity collidingCube);
	glm::vec3 Move(GLFWwindow *window);

	bool IsHoldingObject() { return holdingObject; }
	int GetEntityID() const { return entityID; }

};