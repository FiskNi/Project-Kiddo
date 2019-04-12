#pragma once
#include "Headers.h"

class Character
{
private:

	glm::vec3 position;
	float rotation;

public:
	Character(GLuint shaderProgram);
	~Character();

	void moveCharacter(GLFWwindow* window, float dTime);

};

