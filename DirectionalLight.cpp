#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	this->lightPos = glm::vec3(-17.0, 12.0, 20.0);
	this->direction = lightPos;
	this->diffuse = glm::vec3(1.0f, 0.82f, 0.6f);
	this->specular = glm::vec3(0.5f, 0.5f, 0.5f);

	// Light strength
	float str = 0.8f;
	diffuse *= str;
}

DirectionalLight::DirectionalLight(glm::vec3 lightPos, glm::vec3 direction, float intensity)
{
	this->lightPos = lightPos;
	this->direction = direction;
	this->diffuse = glm::vec3(1.0f, 0.82f, 0.6f);
	this->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	float str = intensity;
	diffuse *= str;
}

DirectionalLight::~DirectionalLight()
{
}

glm::vec3 DirectionalLight::getPos() const
{
	return this->lightPos;
}

glm::vec3 DirectionalLight::getDirection() const
{ 
	return this->direction;
}

void DirectionalLight::setPos(glm::vec3 lightPos)
{
	this->lightPos = lightPos;
}

void DirectionalLight::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void DirectionalLight::setDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void DirectionalLight::setSpecular(glm::vec3 spec)
{
	this->specular = spec;
}

void DirectionalLight::sendToShader(Shader shader)
{
	string name = "dirLight.pos";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->lightPos));
	name = "dirLight.dir";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->direction));
	name = "dirLight.diffuse";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->diffuse));
	name = "dirLight.specular";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->specular));
	
}
