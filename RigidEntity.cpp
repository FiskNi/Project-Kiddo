#include "RigidEntity.h"



RigidEntity::RigidEntity(unsigned int i) : Entity(i)
{
	startPos = glm::vec3(0.0f, 0.0f, 0.0f);
	savedPos = startPos;
	velocity = glm::vec3(0.0f, 10.0f, 0.0f);
	collision = false;

	grounded = false;
	groundLevel = 0.0f;
	held = false;
}


RigidEntity::RigidEntity(Vertex* vertArr, unsigned int nrOfVerticies, unsigned int matID) : Entity(vertArr, nrOfVerticies, matID)
{
	startPos = GetPosition();
	velocity = glm::vec3(0.0f, 10.0f, 0.0f);
	collision = false;
	grounded = false;
	groundLevel = 0.0f;
	held = false;
}


RigidEntity::~RigidEntity()
{
}

void RigidEntity::AddVelocity(float x, float y, float z)
{
	velocity += glm::vec3(x, y, z);
}

void RigidEntity::AddVelocity(glm::vec3 vec)
{
	velocity += vec;
}

void RigidEntity::AddVelocityX(float x)
{
	velocity.x += x;
}

void RigidEntity::AddVelocityY(float y)
{
	velocity.y += y;
}

void RigidEntity::AddVelocityZ(float z)
{
	velocity.z += z;
}

void RigidEntity::SetVelocity(float x, float y, float z)
{
	velocity = glm::vec3(x, y, z);
}

void RigidEntity::SetVelocity(glm::vec3 vec)
{
	velocity = vec;
}

void RigidEntity::SetVelocityX(float x)
{
	velocity.x = x;
}

void RigidEntity::SetVelocityY(float y)
{
	velocity.y = y;
}

void RigidEntity::SetVelocityZ(float z)
{
	velocity.z = z;
}

void RigidEntity::AddRotation(float x, float y, float z)
{
}

void RigidEntity::AddRotationX(float x)
{

}

void RigidEntity::AddRotationY(float y)
{

}

void RigidEntity::AddRotationZ(float z)
{

}

void RigidEntity::Update(float deltaTime)
{
	// Get the current position
	glm::vec3 calculatedPosition = GetPosition();
	calculatedPosition += velocity * deltaTime;

	// Constant global friction
	const float friction = 0.8f;
	if (grounded)
		velocity *= friction;

	if (fabsf(velocity.x) < 0.001f) 
		velocity.x = 0.0f;

	if (fabsf(velocity.y) < 0.001f) 
		velocity.y = 0.0f;
	
	if (fabsf(velocity.z) < 0.001f) 
		velocity.z = 0.0f;

	if (grounded)
	{
		// Hitbox center required further testing
		float bbBottom = GetHitboxSize().y;
		float bbCenter = GetHitboxOffset().y;
		velocity.y = 0.0f;
		calculatedPosition.y = groundLevel + bbBottom - bbCenter;
	}

	// Move this entity
	//savedPos = GetPosition();
	SetPosition(calculatedPosition);

	// This has to be lower than the initial ground level or bad things happen
	if (GetPosition().y < -3.0f)
	{
		ResetPos();
	}
	
}

void RigidEntity::ResetPos()
{
	SetPosition(startPos);	
	SetVelocity(0.0f, 1.0f, 1.0f);
}

void RigidEntity::SetColliding(bool colliding)
{
	collision = colliding;
}

void RigidEntity::SetGrounded(bool grounded)
{
	this->grounded = grounded;
}

void RigidEntity::GroundLevel(float y)
{
	groundLevel = y;
}

void RigidEntity::SetHeld(bool holding)
{
	held = holding;
}

void RigidEntity::SetStartPosition(glm::vec3 pos)
{
	startPos = pos;
}

