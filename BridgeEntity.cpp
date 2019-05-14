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

BridgeEntity::BridgeEntity(Vertex* vertArr, unsigned int vertexCount, unsigned int matID) : StaticEntity(vertArr, vertexCount, matID)
{
	restPosition = GetPosition();
	extendDistance = 2.0f;
	extendDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	extended = false;
	extending = false;
	linkID = -999;
}

BridgeEntity::BridgeEntity(Loader* inLoader, unsigned int index, unsigned int matID) : StaticEntity(inLoader, index, matID)
{
	restPosition = GetPosition();
	extendDistance = 2.0f;
	extendDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	extended = false;
	extending = false;
	linkID = -999;
}

BridgeEntity::BridgeEntity(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen) : StaticEntity(inLoader, index, matID, frozen)
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

void BridgeEntity::SetExtendingDir(int dir)
{
	switch (dir)
	{
	case 1: //Positive x
		extendDirection = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case 2: //Negative x
		extendDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
		break;
	case 3: //Positive z
		extendDirection = glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	case 4: //Negative z
		extendDirection = glm::vec3(0.0f, 0.0f, -1.0f);
		break;
	case 5: //Positive y
		extendDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case 6: //Negative y
		extendDirection = glm::vec3(0.0f, -1.0f, 0.0f);
		break;
	default:
		std::cout << "Direction parameter was incorrect" << std::endl;
		break;
	}
}

void BridgeEntity::SetLinkID(int id)
{
	linkID = id;
}

bool BridgeEntity::CheckLinkID(int id)
{
	if (id == linkID)
		return true;

	return false;
}

void BridgeEntity::Extend()
{
	if (!extending)
		extending = true;
}

void BridgeEntity::Retract()
{
	extended = false;
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
			{
				if ((restPosition + (extendDirection * extendDistance)).x > GetPosition().x)
				{
					SetPosition(calculatedPosition);
				}
				else
				{
					extended = true;
					extending = false;
				}
			}
			if (extendDirection == glm::vec3(-1.0f, 0.0f, 0.0f))
			{
				if ((restPosition + (extendDirection * extendDistance)).x < GetPosition().x)
				{
					SetPosition(calculatedPosition);
				}
				else
				{
					extended = true;
					extending = false;
				}
			}
			if (extendDirection == glm::vec3(0.0f, 0.0f, 1.0f))
			{
				if ((restPosition + (extendDirection * extendDistance)).z > GetPosition().z)
				{
					SetPosition(calculatedPosition);
				}
				else
				{
					extended = true;
					extending = false;
				}
			}
			if (extendDirection == glm::vec3(0.0f, 0.0f, -1.0f))
			{
				if ((restPosition + (extendDirection * extendDistance)).z < GetPosition().z)
				{
					SetPosition(calculatedPosition);
				}
				else
				{
					extended = true;
					extending = false;
				}
			}

			if (extendDirection == glm::vec3(0.0f, 1.0f, 0.0f))
			{
				if ((restPosition + (extendDirection * extendDistance)).y < GetPosition().y)
				{
					SetPosition(calculatedPosition);
				}
				else
				{
					extended = true;
					extending = false;
				}
			}	
			if (extendDirection == glm::vec3(0.0f, -1.0f, 0.0f))
			{
				if ((restPosition + (extendDirection * extendDistance)).y < GetPosition().y)
				{
					SetPosition(calculatedPosition);
				}
				else
				{
					extended = true;
					extending = false;
				}
			}
		}
	}
	else if (!extended)
	{
		SetPosition(restPosition);
		extended = false;
	}
}
