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
	StaticEntity(vertex * vertArr, unsigned int nrOfVerticies);
	~StaticEntity();

};
