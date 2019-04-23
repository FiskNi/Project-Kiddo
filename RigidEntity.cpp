#include "RigidEntity.h"



RigidEntity::RigidEntity(unsigned int i) : Entity(i)
{
	speed = 0.0f;
	collision = false;
	grounded = false;
	held = false;
}


RigidEntity::~RigidEntity()
{
}

void RigidEntity::setSpeed(float speed)
{
	this->speed = speed;
}

void RigidEntity::setColliding(bool colliding)
{
	collision = colliding;
}

void RigidEntity::setGrounded(bool grounded)
{
	this->grounded = grounded;
}

void RigidEntity::setHeld(bool holding)
{
	held = holding;
}

float RigidEntity::getSpeed() const
{
	return speed;
}

bool RigidEntity::isColliding() const
{
	return collision;
}

bool RigidEntity::isGrounded() const
{
	return grounded;
}

bool RigidEntity::isHeld() const
{
	return held;
}
