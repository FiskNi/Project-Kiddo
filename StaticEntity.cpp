#include "StaticEntity.h"

StaticEntity::StaticEntity(unsigned int i) : Entity(i)
{
}

StaticEntity::StaticEntity(Vertex* vertArr, unsigned int nrOfVerticies, unsigned int matID) : Entity(vertArr, nrOfVerticies, matID)
{

}

StaticEntity::~StaticEntity()
{

}


