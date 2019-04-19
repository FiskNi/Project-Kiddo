#pragma once
#include "Headers.h"
#include "Primitive.h"

class Entity
{
private:
	Primitive entityMesh;
	glm::vec3 boundingBoxSize;

	unsigned int materialID;
	bool collision;

public:
	Entity();
	~Entity();

	bool CheckCollision(Entity collidingCube);

	void setMaterialID(unsigned int materialID);
	void setPosition(glm::vec3 newPos);

	void setCustomBBox(glm::vec3);

	Primitive getMeshData() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSize() const;
};

