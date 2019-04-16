#pragma once

#include "Headers.h"
#include "Shader.h"

class DirLight
{
private:
	glm::vec3 lightPos;
	glm::vec3 direction;
	glm::vec3 lightCol;
	glm::vec3 specular;

public:

	DirLight();
	~DirLight();

	glm::vec3 getLightPos();
	glm::vec3 getDirection();

	void setLightPos(glm::vec3 lightPos);
	void setDirection(glm::vec3 direction);
	void setLightCol(glm::vec3 lightCol);
	void setSpecular(glm::vec3 spec);

	void sendToShader(Shader shader);
};
