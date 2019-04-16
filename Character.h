#pragma once
#include "Headers.h"
#include "Primitive.h"
#include "Entity.h"

class Character
{
private:

	Primitive playerMesh;
	glm::vec3 boundingBoxSize;
	glm::vec3 position;
	float rotation;

public:
	Character();
	~Character();

	Primitive getMeshData() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSize() const;

	void setPosition(glm::vec3 newPos);
	bool CheckCollision(Entity collidingCube);
	glm::vec3 Move(GLFWwindow *window, float deltaTime);

private:
	glm::vec3 calcMovement(float, float, float);
};

