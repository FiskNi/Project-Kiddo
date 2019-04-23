#pragma once
#include "Headers.h"
#include "Primitive.h"
#include "Entity.h"

class Character
	: public Entity
{
private:

	Primitive playerMesh;
	glm::vec3 boundingBoxSize;
	glm::vec3 position;
	float rotation;

	//ID, playerID = 1, Movable box = 2.
	unsigned int entityID;

	unsigned int materialID;

public:
	Character();
	~Character();

	Primitive getMeshData() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSize() const;

	void setMaterialID(unsigned int id);
	void setPosition(glm::vec3 newPos);
	bool CheckCollision(Entity collidingCube);
	bool CheckInBound(Entity collidingCube);
	glm::vec3 Move(GLFWwindow *window, float deltaTime);
};