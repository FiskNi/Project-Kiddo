#pragma once
#include "Headers.h"

//============================================================================
//	- Primitive
//	Handles pure mesh data (vertices and currently material)
//	This is where everything the renderer needs for the shader inputs and nothing else.
//		*Will be renamed to "Mesh" as this will do the main mesh handling.
//		Primitives will be a fallback on meshloading failure
//============================================================================

class Mesh
{
private:
	glm::vec3 worldPosition;
	glm::vec3 worldRotation;


	unsigned int nrOfVerticies;
	std::vector<vertexPolygon> vertices;

	unsigned int materialID;

public:
	Mesh(vertex* vertArr, unsigned int nrOfVerticies);
	Mesh();
	~Mesh();

	void CreateCubeData();
	void CreatePlaneData();
	void ImportMesh(vertex* verticies, int nrOfVerticies);

	void CalculateTangents();
	
	void setMaterial(unsigned int texID);

	void setPosition(glm::vec3 newPos);

	void SetRotation(float x, float y, float z);
	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);

	unsigned int getMaterialID() const;
	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;

	std::vector<vertexPolygon> getvertexPolygons();
	unsigned int GetVertexCount() const { return nrOfVerticies; }

	int getPolygonCount() const;
};

