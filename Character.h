#pragma once
#include "Headers.h"
#include "Mesh.h"
#include "RigidEntity.h"


//============================================================================
//	- Character
//	The character as can be seen derives from a rigid entity.
//	This means any physic updates based on velocty etc, can easily be appplied
//	on the character as well as any moveable objects.
//============================================================================
class Character
	: public RigidEntity
{
private:
	//ID, playerID = 1, Movable box = 2.
	int entityID;
	bool holdingObject;

	glm::vec3 inputVector;
	bool jumpSquat = false;

public:
	Character();
	~Character();
	
	void SetHoldingObject(bool holding);
	void SetEntityID(unsigned int id);

	bool CheckInBound(Entity collidingCube);
	glm::vec3 Move(GLFWwindow *window);

	bool IsHoldingObject() { return holdingObject; }
	int GetEntityID() const { return entityID; }
	glm::vec3 GetInputVector() { return inputVector; }

};