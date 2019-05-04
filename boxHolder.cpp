#include "boxHolder.h"

boxHolder::boxHolder(unsigned int i) : StaticEntity (i)
{
	SetPosition(glm::vec3(5, -1, 0));
	origPos = GetPosition();
	origBBPos = GetPositionBB();
	setPositionBBOffset(GetPositionBB());
}

boxHolder::boxHolder(vertex * vertArr, unsigned int nrOfVerticies) : StaticEntity (vertArr, nrOfVerticies)
{
	SetPosition(glm::vec3(5, -1, 0));
	origPos = GetPosition();
	origBBPos = GetPositionBB();
	setPositionBBOffset(origBBPos);
}

boxHolder::~boxHolder()
{
}

void boxHolder::puntBox()
{
	SetPosition(glm::vec3(999, 0, 0));
}

void boxHolder::boxReturn()
{
	SetPosition(this->origPos);
}

