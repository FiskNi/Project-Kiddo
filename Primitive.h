#pragma once
#include "Headers.h"

class Primitive
{
private:
	GLuint diffuseID;
	GLuint normalID;
	glm::vec3 worldPosition;
	float worldRotation;

	std::vector<vertexPolygon> vertices;

	GLuint gVertexBuffer;
	GLuint gVertexAttribute;

public:
	Primitive();
	~Primitive();

	void CreateCubeData();
	void CreatePlaneData();
	
	void setTextureID(GLuint texID);

	GLuint getTextureID() const;
	GLuint getVertexAttribute() const;
	glm::vec3 getWorldPosition() const;
	float getWorldRotation() const;

	void MovePrimitive(GLFWwindow *window, float deltaTime);
	void setPosition();
	std::vector<vertexPolygon> getvertexPolygons();
	unsigned int getPolygonCount() const;
};

