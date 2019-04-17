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

	bool CheckCollision(Entity collidingCube);

	void setTextureID(GLuint texID);
	void setPosition(glm::vec3 newPos);

	Primitive getMeshData() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSize() const;
};

