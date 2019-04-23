#pragma once
#include "Entity.h"

//============================================================================
//	- Rigids
//	Moveable entities like boxes 
//============================================================================

class RigidEntity : public Entity
{
private:
	float speed;

	bool collision;
	bool grounded;
	bool held;


public:
	RigidEntity(unsigned int i);
	~RigidEntity();

	void setSpeed(float speed);
	void setColliding(bool colliding);
	void setGrounded(bool grounded);
	void setHeld(bool holding);

	float getSpeed() const;
	bool isColliding() const;
	bool isGrounded() const;
	bool isHeld() const;
};

