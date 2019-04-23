#pragma once
#include "Entity.h"

class RigidEntity : public Entity
{
private:
	float speed;
	bool collision;
	bool grounded;


public:
	RigidEntity(unsigned int i);
	~RigidEntity();

	void setSpeed(float speed);
	void setColliding(bool colliding);
	void setGrounded(bool grounded);

	float getSpeed() const;
	bool isColliding() const;
	bool isGrounded() const;
};

