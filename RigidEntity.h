#pragma once
#include "Entity.h"

//============================================================================
//	- Rigids
//	Moveable entities like boxes 
//============================================================================

class RigidEntity : public Entity
{
private:
	glm::vec3 velocity;

	bool collision;
	bool grounded;
	float groundLevel;
	bool held;


public:
	RigidEntity(unsigned int i);
	RigidEntity(vertex * vertArr, unsigned int nrOfVerticies);
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

	void SetColliding(bool colliding);
	void SetGrounded(bool grounded);
	void GroundLevel(float y);
	void SetHeld(bool holding);

	glm::vec3 GetVelocity() const { return velocity; }
	float GetVelocityX() const { return velocity.x; }
	float GetVelocityY() const { return velocity.y; }
	float GetVelocityZ() const { return velocity.z; }
	float GetGroundLevel() const { return groundLevel; }

	bool IsColliding() const { return collision; }
	bool IsGrounded() const { return grounded; }
	bool IsHeld() const { return held; }
};

