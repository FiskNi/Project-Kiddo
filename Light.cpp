#include "Light.h"


Light::Light()
{
	this->lightPos = glm::vec3(0.0f, 2.0, 0.0f);
	this->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	this->specular = glm::vec3(1.0f, 1.0f, 1.0f);

	this->constant = 1.0f;
	this->linear = 0.35f;
	this->quadratic = 0.44f;
	this->range = 11.0f;
	this->power = 10.0f;
}

Light::~Light()
{
}

glm::vec3 Light::getLightPos()
{
	return this->lightPos;
}

void Light::setLightPos(glm::vec3 lightPos)
{
	this->lightPos = lightPos;
}

void Light::setDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void Light::setSpecular(glm::vec3 specular)
{
	this->specular = specular;
}

void Light::setAttenuation(float constant, float linear, float quadratic)
{
	this->constant = constant;
	this->linear = linear;
	this-> quadratic = quadratic;
}

void Light::setRange(float range)
{
	this->range = range;
}

void Light::setPower(float power)
{
	this->power = power;
}

void Light::sendToShader(Shader shader, int index)
{
	glUseProgram(shader.getShader());

	string name = "pointLights[" + to_string(index) + "].pos";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->lightPos));

	name = "pointLights[" + to_string(index) + "].diffuse";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->diffuse));

	name = "pointLights[" + to_string(index) + "].specular";
	glUniform3fv(glGetUniformLocation(shader.getShader(), name.c_str()), 1, glm::value_ptr(this->specular));

	name = "pointLights[" + to_string(index) + "].constant";
	glUniform1f(glGetUniformLocation(shader.getShader(), name.c_str()), this->constant);

	name = "pointLights[" + to_string(index) + "].linear";
	glUniform1f(glGetUniformLocation(shader.getShader(), name.c_str()), this->linear);

	name = "pointLights[" + to_string(index) + "].quadratic";
	glUniform1f(glGetUniformLocation(shader.getShader(), name.c_str()), this->constant);

	name = "pointLights[" + to_string(index) + "].range";
	glUniform1f(glGetUniformLocation(shader.getShader(), name.c_str()), this->range);

	name = "pointLights[" + to_string(index) + "].power";
	glUniform1f(glGetUniformLocation(shader.getShader(), name.c_str()), this->power);
}




