#pragma once
#include "Headers.h"

class CreatePrimitive
{
private:
	GLuint gVertexBuffer;
	GLuint gVertexAttribute;
	GLuint diffuseID;
	GLuint normalID;
	glm::vec3 worldPosition;
	float worldRotation;

	std::vector<vertexPolygon> vertices;

public:
	CreatePrimitive();
	~CreatePrimitive();

	void CreateTriangleData(GLuint shaderID, float test);
	
	void setTextureID(GLuint texID);

	GLuint getTextureID() const;
	GLuint getVertexAttribute() const;
	glm::vec3 getWorldPosition() const;
	float getWorldRotation() const;

	void MovePrimitive(GLFWwindow *window, float deltaTime);
	void setPosition();
	std::vector<vertexPolygon> getvertexPolygons();
};

