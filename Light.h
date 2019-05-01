#pragma once
#include "Headers.h"
#include "Shader.h"

/*	Attenuation cheat chart (attModifier)
Slot	Range	Const	Linear	Quadratic
1		7		1.0		0.7		1.8
2		13		1.0		0.35	0.44
3		20		1.0		0.22	0.20
4		32		1.0		0.14	0.07
5		50		1.0		0.09	0.032
6		65		1.0		0.07	0.017
7		100		1.0		0.045	0.0075
8		160		1.0		0.027	0.0028
9		200		1.0		0.022	0.0019
10		325		1.0		0.014	0.0007
11		600		1.0		0.007	0.0002
12		3250	1.0		0.0014	0.000007
*/

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
	Light(glm::vec3 pos, float strength, float range, int attModifier);
	Light(float x, float y, float z, float strength, float range, int attModifier);
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

