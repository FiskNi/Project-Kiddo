#pragma once
#include "Headers.h"
#include "Primitive.h"

class Entity
{
private:
	Primitive entityMesh;
	glm::vec3 boundingBoxSize;

public:
	Entity();
	~Entity();

	Primitive getMeshData() const;
	glm::vec3 getPosition() const;

	void setPosition(glm::vec3 newPos);
	void CheckCollision(glm::vec3 position, glm::vec3 size);
	void Move(GLFWwindow *window, float deltaTime);
};

