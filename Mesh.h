#pragma once
#include "Headers.h"

//============================================================================
//	- Mesh
//	Handles pure mesh data.
//	This is where everything the renderer needs for the shader inputs and nothing else.
//	Currently the mesh also holds onto the material id. This could be changed to make
//	entities hold their own material id.
//============================================================================

class Mesh
{
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	int nrOfVerticies;
	std::vector<vertexPolygon> vertices;

	unsigned int materialID;

public:
	Mesh(Vertex* vertArr, unsigned int nrOfVerticies);
	Mesh();
	~Mesh();

	void CreateCubeData();
	void CreatePlaneData();
	void CreatePlateData();
	void ImportMesh(Vertex* verticies, int nrOfVerticies);

	void CalculateTangents();
	
	void setMaterial(unsigned int texID);

	void setPosition(glm::vec3 newPos);
	void setPosition(float x, float y, float z);
	void setPositionX(float x);
	void setPositionY(float y);
	void setPositionZ(float z);

	void SetRotation(glm::vec3 newRot);
	void SetRotation(float x, float y, float z);
	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);

	void SetScale(glm::vec3 newSca);
	void SetScale(float x, float y, float z);


	unsigned int getMaterialID() const;
	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;

	std::vector<vertexPolygon> GetVertices();
	std::vector<vertexPolygon>& ModifyVertices();
	unsigned int GetVertexCount() const { return nrOfVerticies; }

	int getVertexCount() const;
};

