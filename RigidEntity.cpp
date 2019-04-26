#include "RigidEntity.h"



RigidEntity::RigidEntity(unsigned int i) : Entity(i)
{
	velocity = glm::vec3(0.0f, 10.0f, 0.0f);
	collision = false;
	grounded = false;
	held = false;
}


RigidEntity::~RigidEntity()
{
}

void RigidEntity::AddVelocity(float x, float y, float z)
{
	this->velocity += glm::vec3(x, y, z);
}

void RigidEntity::AddVelocity(glm::vec3 vec)
{
	this->velocity += vec;
}

void RigidEntity::AddVelocityX(float x)
{
	this->velocity.x += x;
}

void RigidEntity::AddVelocityY(float y)
{
	this->velocity.y += y;
}

void RigidEntity::AddVelocityZ(float z)
{
	this->velocity.z += z;
}

void RigidEntity::SetVelocity(float x, float y, float z)
{
	this->velocity = glm::vec3(x, y, z);
}

void RigidEntity::SetVelocity(glm::vec3 vec)
{
	this->velocity = vec;
}

void RigidEntity::SetVelocityX(float x)
{
	this->velocity.x = x;
}

void RigidEntity::SetVelocityY(float y)
{
	this->velocity.y = y;
}

void RigidEntity::SetVelocityZ(float z)
{
	this->velocity.z = z;
}

void RigidEntity::Update(float deltaTime)
{
	// Get the current position
	glm::vec3 calculatedPosition = GetPosition();

	// Constant global friction
	const float friction = 0.8f;
	if (grounded)
		velocity *= friction;



	if (fabsf(velocity.x) < 0.001f) {
		velocity.x = 0.0f;
	}
	if (fabsf(velocity.y) < 0.001f) {
		velocity.y = 0.0f;
	}
	if (fabsf(velocity.z) < 0.001f) {
		velocity.z = 0.0f;
	}

	calculatedPosition += velocity * deltaTime;
	SetPosition(calculatedPosition);

	this->resetPos();
}

void RigidEntity::resetPos()
{
	if (GetPosition().y < -10 && GetPosition().y > -45) {
		this->velocity.y, this->velocity.x = 0;
		this->SetPosition(this->startPos);	
	}
}

void RigidEntity::SetColliding(bool colliding)
{
	this->collision = colliding;
}

void RigidEntity::SetGrounded(bool grounded)
{
	this->grounded = grounded;
}

void RigidEntity::SetHeld(bool holding)
{
	this->held = holding;
}

void RigidEntity::setStartPosition(glm::vec3 pos)
{
	this->startPos = pos;
}

