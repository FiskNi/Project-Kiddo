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
	StaticEntity(Vertex * vertArr, unsigned int nrOfVerticies, unsigned int matID);
	StaticEntity(Loader* inLoader, unsigned int index, unsigned int matID);
	~StaticEntity();
};

