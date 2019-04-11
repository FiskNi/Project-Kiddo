#pragma once
#include "Headers.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class CreatePrimitive
{
private:
	GLuint gVertexBuffer;
	GLuint gVertexAttribute;
	glm::vec3 worldPosition;
	float worldRotation;

	glm::vec3 pos;

public:
	CreatePrimitive();
	~CreatePrimitive();

	void CreateTriangleData(GLuint shaderID, float test);
	

	GLuint getVertexAttribute() const;
	glm::vec3 getWorldPosition() const;
	float getWorldRotation() const;

	void MovePrimitive(GLFWwindow *window, float deltaTime);
	void setPosition();
};

