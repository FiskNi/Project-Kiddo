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
	glm::vec3 getPosition() const;
	float getWorldRotation() const;

	void setPosition(glm::vec3 newPos);
	std::vector<vertexPolygon> getvertexPolygons();
	unsigned int getPolygonCount() const;
};

