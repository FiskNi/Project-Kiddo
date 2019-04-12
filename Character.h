#pragma once
#include "Headers.h"
#include "CreatePrimitives.h"
class Character
{
private:
	CreatePrimitive characterModel;

	glm::vec3 position;
	float rotation;

public:
	Character(GLuint shaderProgram);
	~Character();

	void moveCharacter(GLFWwindow* window, float dTime);

	CreatePrimitive getMesh();
};

