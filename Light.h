#pragma once

#include "Headers.h"

class Light
{

private:
	glm::vec3 lightPos;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant;
	//Radius?

public:

	float linear;
	float quadratic;

	Light();
	//Light(glm::vec3 lightPos, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	~Light();

	glm::vec3 getLightPos();
};

