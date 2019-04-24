#pragma once
#include "Headers.h"

//============================================================================
//	- Primitive
//	Handles pure mesh data (vertices and currently material)
//	This is where everything the renderer needs for the shader inputs and nothing else.
//		*Will be renamed to "Mesh" as this will do the main mesh handling.
//		Primitives will be a fallback on meshloading failure
//============================================================================

class Primitive
{
private:
	glm::vec3 worldPosition;
	float worldRotation;

	std::vector<vertexPolygon> vertices;
	GLuint gVertexBuffer;
	GLuint gVertexAttribute;

	vertex * importVerts;

	unsigned int materialID;

public:
	Primitive();
	~Primitive();

	void CreateCubeData();
	void CreatePlaneData();

	void ImportMesh(vertex*verticies, int nrOfVerticies);

	void CalculateTangents();
	
	void setMaterial(unsigned int texID);

	unsigned int getMaterialID() const;
	GLuint getVertexAttribute() const;
	glm::vec3 getPosition() const;
	float getRotation() const;

	void setPosition(glm::vec3 newPos);
	std::vector<vertexPolygon> getvertexPolygons();
	unsigned int getPolygonCount() const;
};

