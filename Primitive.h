#pragma once
#include "Headers.h"

class Primitive
{
private:
	glm::vec3 worldPosition;
	float worldRotation;

	std::vector<vertexPolygon> vertices;
	GLuint gVertexBuffer;
	GLuint gVertexAttribute;

	GLuint materialID;

public:
	Primitive();
	~Primitive();

	void CreateCubeData();
	void CreatePlaneData();

	void CalculateTangents();
	
	void setMaterial(GLuint texID);

	unsigned int getMaterialID() const;
	GLuint getVertexAttribute() const;
	glm::vec3 getPosition() const;
	float getWorldRotation() const;

	void setPosition(glm::vec3 newPos);
	std::vector<vertexPolygon> getvertexPolygons();
	unsigned int getPolygonCount() const;
};

