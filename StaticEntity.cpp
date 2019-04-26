#include "StaticEntity.h"



StaticEntity::StaticEntity(unsigned int i) : Entity(i)
{
}

StaticEntity::StaticEntity(vertex* vertArr, unsigned int nrOfVerticies) : Entity(vertArr, nrOfVerticies)
{

}

StaticEntity::~StaticEntity()
{
}


