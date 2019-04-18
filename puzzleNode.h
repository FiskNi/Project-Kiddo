#pragma once
#include "Entity.h"
class puzzleNode :
	public Entity
{
public:
	puzzleNode();
	~puzzleNode();

	bool checkInside(Entity collidingCube);
};

