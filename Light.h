#pragma once
#include "Headers.h"
#include "Shader.h"

class Light
{

private:
	glm::vec3 lightPos;
	// Diffuse is Light Colour.
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float range;
	float power;

public:
	Light();
	~Light();

	glm::vec3 getLightPos();
	void setLightPos(glm::vec3 lightPos);
	void setDiffuse(glm::vec3 diffuse);
	void setSpecular(glm::vec3 specular);
	void setAttenuation(float constant, float linear, float quadratic);
	void setRange(float range);
	void setPower(float power);

	//Use this function to send the specific pointLight to the specific shader.
	//Putting it at "index" location in pointLight uniform.
	void sendToShader(Shader shader, int index);

};

