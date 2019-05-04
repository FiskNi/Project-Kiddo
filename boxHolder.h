#pragma once

#include "StaticEntity.h"

class boxHolder : public StaticEntity
{
private:
	glm::vec3 origPos;
	glm::vec3 origBBPos;
	
public:
	boxHolder(unsigned int i);
	boxHolder(vertex * vertArr, unsigned int nrOfVerticies);
	~boxHolder();

	void puntBox();
	void boxReturn();


};
