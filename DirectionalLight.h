#pragma once

#include "Headers.h"
#include "Shader.h"

class DirectionalLight
{
private:
	glm::vec3 lightPos;
	glm::vec3 direction;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float strength;

public:
	DirectionalLight();
	DirectionalLight(glm::vec3 lightPos, glm::vec3 direction, float intensity);
	~DirectionalLight();

	glm::vec3 GetPos() const;
	glm::vec3 GetDirection() const;

	void SetPos(glm::vec3 lightPos);
	void SetDirection(glm::vec3 direction);
	void SetStrength(float str);
	void SetDiffuse(glm::vec3 lightCol);
	void SetSpecular(glm::vec3 spec);

	void SendToShader(Shader shader);
};
