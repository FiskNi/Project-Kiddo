#pragma once

#include "StaticEntity.h"

class boxHolder
{
private:
	glm::vec3 worldPos;
	glm::vec3 rotation;

	std::vector<StaticEntity> bridge;
	
public:
	boxHolder();
	~boxHolder();
};
