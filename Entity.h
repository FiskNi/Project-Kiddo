#pragma once
#include "Headers.h"
#include "Primitive.h"

class Entity
{
private:
	Primitive entityMesh;
	glm::vec3 boundingBoxSize;

	bool collision;

public:
	Entity();
	~Entity();

	Primitive getMeshData() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSize() const;

	void setPosition(glm::vec3 newPos);
	bool CheckCollision(Entity collidingCube);
};

