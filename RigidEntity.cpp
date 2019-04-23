#include "RigidEntity.h"



RigidEntity::RigidEntity(unsigned int i) : Entity(i)
{
	speed = 0.0f;
	collision = false;
	grounded = false;
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
