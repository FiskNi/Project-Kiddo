#include "StaticEntity.h"

StaticEntity::StaticEntity(unsigned int i) : Entity(i)
{
}

StaticEntity::StaticEntity(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen) : Entity(inLoader, index, matID, frozen)
{

}

StaticEntity::StaticEntity(Loader* inLoader, unsigned int index, unsigned int matID) : Entity(inLoader, index, matID)
{

}

StaticEntity::~StaticEntity()
{

}


