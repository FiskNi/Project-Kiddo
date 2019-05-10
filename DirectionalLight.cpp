#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	this->lightPos = glm::vec3(-17.0, 12.0, 20.0);
	this->direction = lightPos;
	this->diffuse = glm::vec3(1.0f, 0.82f, 0.6f);
	this->specular = glm::vec3(0.5f, 0.5f, 0.5f);

	// Light strength
	strength = 0.8f;
	diffuse *= strength;
}

DirectionalLight::DirectionalLight(glm::vec3 lightPos, glm::vec3 direction, float intensity)
{
	this->lightPos = lightPos;
	this->direction = direction;
	this->diffuse = glm::vec3(1.0f, 0.82f, 0.6f);
	this->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	strength = intensity;
	diffuse *= strength;
}

DirectionalLight::~DirectionalLight()
{
}

glm::vec3 DirectionalLight::GetPos() const
{
	return this->lightPos;
}

glm::vec3 DirectionalLight::GetDirection() const
{ 
	return this->direction;
}

void DirectionalLight::SetPos(glm::vec3 lightPos)
{
	this->lightPos = lightPos;
	this->direction = lightPos;
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void DirectionalLight::SetStrength(float str)
{
	strength = str;
}

void DirectionalLight::SetDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void DirectionalLight::SetSpecular(glm::vec3 spec)
{
	this->specular = spec;
}

void DirectionalLight::SendToShader(Shader shader)
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
