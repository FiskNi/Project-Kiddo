#pragma once
#include "Headers.h"

class CreatePrimitive
{
private:
	GLuint diffuseID;
	GLuint normalID;
	glm::vec3 worldPosition;
	float worldRotation;

	std::vector<vertexPolygon> vertices;

public:
	CreatePrimitive();
	~CreatePrimitive();

	void CreateTriangleData();
	void CreatePlaneData();
	
	void setTextureID(GLuint texID);

	GLuint getTextureID() const;
	GLuint getVertexAttribute() const;
	glm::vec3 getWorldPosition() const;
	float getWorldRotation() const;

	void MovePrimitive(GLFWwindow *window, float deltaTime);
	void setPosition();
	std::vector<vertexPolygon> getvertexPolygons();
};

