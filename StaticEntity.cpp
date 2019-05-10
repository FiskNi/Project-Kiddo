#include "StaticEntity.h"

StaticEntity::StaticEntity(unsigned int i) : Entity(i)
{
}

StaticEntity::StaticEntity(Vertex* vertArr, unsigned int vertexCount, unsigned int matID) : Entity(vertArr, vertexCount, matID)
{

}

StaticEntity::StaticEntity(Loader* inLoader, unsigned int index, unsigned int matID) : Entity(inLoader, index, matID)
{

}

StaticEntity::~StaticEntity()
{

}


