#include "BridgeEntity.h"



BridgeEntity::BridgeEntity(unsigned int i) : StaticEntity (i)
{
	restPosition = GetPosition();
}

BridgeEntity::BridgeEntity(vertex* vertArr, unsigned int nrOfVerticies) : StaticEntity(vertArr, nrOfVerticies)
{
	restPosition = GetPosition();
}

BridgeEntity::~BridgeEntity()
{
}

void BridgeEntity::SetRestPosition(glm::vec3 position)
{
	restPosition = position;
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
		SetPosition(GetPosition() + glm::vec3(2.0f, 0.0f, 0.0f));
	else
		SetPosition(restPosition);
}
