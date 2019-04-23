#pragma once
#include "Headers.h"
#include "Primitive.h"

class Entity
{
private:
	Primitive entityMesh;

	glm::vec3 boundingBoxCenter;
	glm::vec3 boundingBoxSize;


	bool collision;

public:
	Entity();
	~Entity();

	void InitBoundingBox();

	bool CheckCollision(Entity collidingCube);

	void setMaterialID(unsigned int materialID);
	void setPosition(glm::vec3 newPos);
	void SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize); // Should be private maybe

	Primitive getMeshData() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSize() const;
};

