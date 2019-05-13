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
	glm::vec3 rotationEulerXYZ;
	glm::quat rotation;
	glm::vec3 scale;

	int vertexCount;
	std::vector<vertexPolygon> vertices;

	unsigned int materialID;

public:
	Mesh(Vertex* vertArr, unsigned int vertexCount);
	Mesh(Vertex* vertArr, unsigned int vertexCount, unsigned int materialID);
	Mesh(Loader* inLoader, int index);
	Mesh();
	~Mesh();

	void CreateCubeData();
	void CreatePlaneData();
	void CreatePlateData();
	void ImportMesh(Vertex* verticies, int vertexCount);

	void CalculateTangents();

	void setMaterial(unsigned int texID);

	void SetPosition(glm::vec3 newPos);
	void SetPosition(float x, float y, float z);
	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPositionZ(float z);

	void SetRotation(glm::quat newRot);

	void SetScale(glm::vec3 newSca);
	void SetScale(float x, float y, float z);

	std::vector<vertexPolygon>& ModifyVertices();

	unsigned int GetMaterialID() const { return materialID; }
	glm::vec3 GetPosition() const { return position; }
	glm::quat GetRotation() const { return rotation; }
	glm::vec3 GetScale() const { return scale; }

	std::vector<vertexPolygon> GetVertices() { return vertices; }
	int GetVertexCount() const { return vertexCount; }
};


