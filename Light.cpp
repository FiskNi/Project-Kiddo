#include "Light.h"


Light::Light()
{
	this->lightPos = glm::vec3(0.0f, 2.0, 0.0f);
	this->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	this->specular = glm::vec3(1.0f, 1.0f, 1.0f);

	this->constant = 1.0f;
	this->linear = 0.35f;
	this->quadratic = 0.44f;
	this->range = 15.0f;
	this->power = 10.0f;
}

Light::Light(glm::vec3 pos, float strength, float range, int attModifier)
{
	this->lightPos = pos;
	this->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	this->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	this->constant = 1.0f;


	this->range = range;
	this->power = strength;
	switch (attModifier)
	{
	case 0:
		this->linear = 0.7f;
		this->quadratic = 1.8f;
		break;
	case 1:
		this->linear = 0.35f;
		this->quadratic = 0.44f;
		break;
	case 2:
		this->linear = 0.22f;
		this->quadratic = 0.20f;
		break;
	case 4:
		this->linear = 0.14f;
		this->quadratic = 0.07f;
		break;
	case 5:
		this->linear = 0.09f;
		this->quadratic = 0.032;
		break;
	case 6:
		this->linear = 0.07f;
		this->quadratic = 0.017;
		break;
	case 7:
		this->linear = 0.045f;
		this->quadratic = 0.0075;
		break;
	case 8:
		this->linear = 0.027f;
		this->quadratic = 0.0028;
		break;
	case 9:
		this->linear = 0.022f;
		this->quadratic = 0.0019;
		break;
	case 10:
		this->linear = 0.014f;
		this->quadratic = 0.0007;
		break;
	case 11:
		this->linear = 0.007f;
		this->quadratic = 0.0002;
		break;
	case 12:
		this->linear = 0.0014f;
		this->quadratic = 0.000007;
		break;
	default:
		this->linear = 0.35f;
		this->quadratic = 0.44;
		break;
	}
}

Light::Light(float x, float y, float z, float strength, float range, int attModifier)
{
	this->lightPos = glm::vec3(x, y ,z);
	this->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	this->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	this->constant = 1.0f;


	this->range = range;
	this->power = strength;
	switch (attModifier)
	{
	case 0:
		this->linear = 0.7f;
		this->quadratic = 1.8f;
		break;
	case 1:
		this->linear = 0.35f;
		this->quadratic = 0.44f;
		break;
	case 2:
		this->linear = 0.22f;
		this->quadratic = 0.20f;
		break;
	case 4:
		this->linear = 0.14f;
		this->quadratic = 0.07f;
		break;
	case 5:
		this->linear = 0.09f;
		this->quadratic = 0.032;
		break;
	case 6:
		this->linear = 0.07f;
		this->quadratic = 0.017;
		break;
	case 7:
		this->linear = 0.045f;
		this->quadratic = 0.0075;
		break;
	case 8:
		this->linear = 0.027f;
		this->quadratic = 0.0028;
		break;
	case 9:
		this->linear = 0.022f;
		this->quadratic = 0.0019;
		break;
	case 10:
		this->linear = 0.014f;
		this->quadratic = 0.0007;
		break;
	case 11:
		this->linear = 0.007f;
		this->quadratic = 0.0002;
		break;
	case 12:
		this->linear = 0.0014f;
		this->quadratic = 0.000007;
		break;
	default:
		this->linear = 0.35f;
		this->quadratic = 0.44;
		break;
	}
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




