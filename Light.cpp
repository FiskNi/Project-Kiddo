#include "Light.h"


Light::Light()
{
	this->lightPos = glm::vec3(0.0f, 0.0f, -1.0f);
	this->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	this->specular = glm::vec3(1.0f, 1.0f, 1.0f);

	this->constant = 1.0f;
	this->linear = 0.09;
	this->quadratic = 0.032;
}

Light::~Light()
{
}

glm::vec3 Light::getLightPos()
{
	return this->lightPos;
}


