#include "DirLight.h"

DirLight::DirLight()
{
	this->lightPos = glm::vec3(4.0, 6.0, 2.0);
	this->direction = glm::vec3(4.0, 6.0, 2.0);
	this->diffuse = glm::vec3(1.0f, 0.82f, 0.6f);
	this->specular = glm::vec3(0.5f, 0.5f, 0.5f);
}

DirLight::~DirLight()
{

}

glm::vec3 DirLight::getPos() const
{
	return this->lightPos;
}

glm::vec3 DirLight::getDirection() const
{ 
	return this->direction;
}

void DirLight::setPos(glm::vec3 lightPos)
{
	this->lightPos = lightPos;
}

void DirLight::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void DirLight::setDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void DirLight::setSpecular(glm::vec3 spec)
{
	this->specular = spec;
}

void DirLight::sendToShader(Shader shader)
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
