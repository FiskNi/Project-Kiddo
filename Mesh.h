#pragma once
#include "Headers.h"
//#include "MeshGroupClass.h"
//============================================================================
//	- Mesh
//	Handles pure mesh data.
//	This is where everything the renderer needs for the shader inputs and nothing else.
//	Currently the mesh also holds onto the material id. This could be changed to make
//	entities hold their own material id.
//============================================================================

#ifndef MESH_H
#define MESH_H

class MeshGroupClass;

class Mesh
{
private:
	glm::vec3 position;
	glm::vec3 rotationEulerXYZ;
	glm::quat rotation;
	glm::vec3 scale;

	int vertexCount;
	std::vector<vertexPolygon> vertices;

	string name;
	string pName;
	bool isChild;

	//-1 is No Parent, 0 is mesh, 1 is group
	int parentType;

	Mesh * myParent;
	MeshGroupClass * myGroupParent;

	glm::vec3 parentPosOffset;
	glm::vec3 parentSizeOffset;

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
	string GetMeshName() const { return name; }
	string GetMeshParentName() const { return pName; }
	bool GetIsChild() const { return isChild; }
	int GetParentType() const { return parentType; }
	glm::vec3 GetParentPosOffset() const { return parentPosOffset; }
	glm::vec3 GetParentSizeOffset() const { return parentSizeOffset; }

	void SetMeshParent(Mesh *parent);
	void SetGroupParent(MeshGroupClass * parent);
	void SetParentPosOffset(glm::vec3 offset);
	void SetParentSizeOffset(glm::vec3 offset);

	Mesh * GetMeshParent() { return myParent; }
	MeshGroupClass * GetGroupParent() { return myGroupParent; }

	std::vector<vertexPolygon> GetVertices() { return vertices; }
	int GetVertexCount() const { return vertexCount; }
};

#endif MESH_H


