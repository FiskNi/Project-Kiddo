#include "BridgeEntity.h"



BridgeEntity::BridgeEntity(unsigned int i) : StaticEntity (i)
{
	restPosition = GetPosition();
	extendDistance = 2.0f;
	extendDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	extended = false;
	extending = false;
	linkID = -999;
}

BridgeEntity::BridgeEntity(vertex* vertArr, unsigned int nrOfVerticies, unsigned int matID) : StaticEntity(vertArr, nrOfVerticies, matID)
{
	restPosition = GetPosition();
	extendDistance = 2.0f;
	extendDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	extended = false;
	extending = false;
	linkID = -999;
}

BridgeEntity::~BridgeEntity()
{
}

void BridgeEntity::SetRestPosition(glm::vec3 position)
{
	restPosition = position;
}

void BridgeEntity::SetRestPosition(float x, float y, float z)
{
	restPosition = glm::vec3(x, y, z);
}

void BridgeEntity::SetExtendDistance(float d)
{
	extendDistance = d;
}

void BridgeEntity::SetExtendingForwardX()
{
	extendDirection = glm::vec3(1.0f, 0.0f, 0.0f);
}

void BridgeEntity::SetExtendingBackwardX()
{
	extendDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
}

void BridgeEntity::SetExtendingForwardZ()
{
	extendDirection = glm::vec3(0.0f, 0.0f, 1.0f);
}

void BridgeEntity::SetExtendingBackwardZ()
{
	extendDirection = glm::vec3(0.0f, 0.0f, -1.0f);
}

bool BridgeEntity::CheckLinkID(int id)
{
	if (id = linkID)
		return true;

	return false;
}

void BridgeEntity::Extend()
{
	extending = true;
}

void BridgeEntity::Retract()
{
	extending = false;
}

void BridgeEntity::Update(float deltaTime)
{
	if (extending)
	{	
		if (!extended)
		{
			const float speed = 5.0f;
			glm::vec3 velocity = extendDirection * speed * deltaTime;
			glm::vec3 calculatedPosition = GetPosition();
			calculatedPosition += velocity;

			glm::vec3 extendStop = restPosition + extendDirection * extendDistance;

			if (extendDirection == glm::vec3(1.0f, 0.0f, 0.0f))
				if ((restPosition + (extendDirection * extendDistance)).x > GetPosition().x)
					SetPosition(calculatedPosition);
				else
					extended = true;

			if (extendDirection == glm::vec3(-1.0f, 0.0f, 0.0f))
				if ((restPosition + (extendDirection * extendDistance)).x < GetPosition().x)
					SetPosition(calculatedPosition);
				else
					extended = true;

			if (extendDirection == glm::vec3(0.0f, 0.0f, 1.0f))
				if ((restPosition + (extendDirection * extendDistance)).z > GetPosition().z)
					SetPosition(calculatedPosition);
				else
					extended = true;

			if (extendDirection == glm::vec3(0.0f, 0.0f, -1.0f))
				if ((restPosition + (extendDirection * extendDistance)).z < GetPosition().z)
					SetPosition(calculatedPosition);
				else
					extended = true;
		}
	}	
	else
	{
		SetPosition(restPosition);
		extended = false;
	}
}
