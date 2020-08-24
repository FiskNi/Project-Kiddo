#pragma once
#include "Entity.h"

//============================================================================
//	- Statics
//	Unmoveable entities like walls or the ground 
//============================================================================

class StaticEntity : public Entity
{
public:
	StaticEntity(unsigned int i);
	StaticEntity(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen);
	StaticEntity(Loader* inLoader, unsigned int index, unsigned int matID);
	~StaticEntity();
};

