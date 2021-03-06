#pragma once
#include "Entity.h"

//============================================================================
//	- Rigids
//	Moveable entities like boxes 
//	Should not be used, make derived classes
//============================================================================

class RigidEntity : public Entity
{
private:
	BOXTYPE boxType = REGULAR;

	glm::vec3 startPos;
	glm::vec3 velocity;
	glm::vec3 savedPos;

	bool collision;
	bool grounded;
	float groundLevel;
	bool held = false;

public:
	RigidEntity(unsigned int i);
	RigidEntity(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen);
	RigidEntity(Loader* inLoader, unsigned int index, unsigned int matID);
	~RigidEntity();

	void AddVelocity(float x, float y, float z);
	void AddVelocity(glm::vec3 vec);
	void AddVelocityX(float x);
	void AddVelocityY(float y);
	void AddVelocityZ(float z);

	void SetVelocity(float x, float y, float z);
	void SetVelocity(glm::vec3 vec);
	void SetVelocityX(float x);
	void SetVelocityY(float y);
	void SetVelocityZ(float z);

	void AddRotation(float x, float y, float z);
	void AddRotationX(float x);
	void AddRotationY(float y);
	void AddRotationZ(float z);

	void Update(float deltaTime);
	void ResetPos();

	void SetColliding(bool colliding);
	void SetGrounded(bool grounded);
	void GroundLevel(float y);
	void SetHeld(bool holding);
	void SetStartPosition(glm::vec3 pos);
	void SetStartPosition(float xyz[]);
	void SetSavedPosition(glm::vec3 pos) { savedPos = pos; }
	void SetBoxType(ITEMTYPE type) { 
		this->boxType = (BOXTYPE)type;
		std::cout << "Box upgraded to: " << this->boxType << std::endl;
	}

	glm::vec3 GetVelocity() const { return velocity; }
	float GetVelocityX() const { return velocity.x; }
	float GetVelocityY() const { return velocity.y; }
	float GetVelocityZ() const { return velocity.z; }
	glm::vec3 GetSavedPos() const { return savedPos; }
	BOXTYPE GetBoxType() { return this->boxType; }

	float GetGroundLevel() const { return groundLevel; }
	bool IsColliding() const { return collision; }
	bool IsGrounded() const { return grounded; }
	bool IsHeld() const { return held; }

	void Upgrade(ITEMTYPE item);
	void BombTimer() {};
};