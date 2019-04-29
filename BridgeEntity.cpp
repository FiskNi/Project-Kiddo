#include "BridgeEntity.h"



BridgeEntity::BridgeEntity(unsigned int i) : StaticEntity (i)
{
	restPosition = GetPosition();
	extendDistance = 2.0f;
	extendDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	extended = false;
	linkID = -999;
}

BridgeEntity::BridgeEntity(vertex* vertArr, unsigned int nrOfVerticies) : StaticEntity(vertArr, nrOfVerticies)
{
	restPosition = GetPosition();
	extendDistance = 2.0f;
	extendDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	extended = false;
	linkID = -999;
}

BridgeEntity::~BridgeEntity()
{
}

void BridgeEntity::SetRestPosition(glm::vec3 position)
{
	restPosition = position;
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
	extended = true;
}

void BridgeEntity::Retract()
{
	extended = false;
}

void BridgeEntity::Update()
{
	if (extended)
		SetPosition(restPosition + (extendDirection * extendDistance));
	else
		SetPosition(restPosition);
}
