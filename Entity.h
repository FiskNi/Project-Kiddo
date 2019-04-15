#pragma once
#include "Headers.h"
#include "Primitive.h"

class Entity
{
private:
	Primitive entityMesh;


public:
	Entity();
	~Entity();

	Primitive getMeshData() const;

	void setPosition(glm::vec3 newPos);
};

