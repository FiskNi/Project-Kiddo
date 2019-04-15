#include "DirLight.h"

DirLight::DirLight()
{
	this->lightPos = glm::vec3(0, 5, 0);
	this->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	this->lightCol = glm::vec3(0.4f, 0.4f, 0.4f);
	this->specular = glm::vec3(0.5f, 0.5f, 0.5f);
}

DirLight::~DirLight()
{

}

glm::vec3 DirLight::getLightPos()
{
	return this->lightPos;
}

glm::vec3 DirLight::getDirection()
{
	return this->direction;
}

void DirLight::setLightPos(glm::vec3 lightPos)
{
	this->lightPos = lightPos;
}

void DirLight::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void DirLight::setLightCol(glm::vec3 lightCol)
{
	this->lightCol = lightCol;
}

void DirLight::setSpecular(glm::vec3 spec)
{
	this->specular = spec;
}

void DirLight::sendToShader(ShaderHandler shader)
{
	string name = "dirLight.pos";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->lightPos));
	name = "dirLight.dir";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->direction));
	name = "dirLight.col";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->lightCol));
	name = "dirLight.specular";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->specular));
	
}
