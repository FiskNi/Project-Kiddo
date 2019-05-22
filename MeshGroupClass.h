#pragma once
#include "Headers.h"
//#include "Mesh.h"
#ifndef MESHGROUPCLASS_H
#define MESHGROUPCLASS_H
#include "Headers.h"
class Mesh;

class MeshGroupClass
{
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	string name;
	string pName;
	bool isChild;

	//-1 is No Parent, 0 is mesh, 1 is group
	int parentType;
	glm::vec3 parentPosOffset;

	Mesh * myMeshParent;
	MeshGroupClass * myGroupParent;

public:
	MeshGroupClass();
	MeshGroupClass(Loader *inLoader, int index);
	~MeshGroupClass();

	void SetPosition(glm::vec3 newPos);
	void SetRotation(glm::vec3 newRot);
	void SetScale(glm::vec3 newScale);

	void SetMeshParent(Mesh* parent);
	void SetGroupParent(MeshGroupClass * parent);

	glm::vec3 GetGroupPosition() { return position; }
	glm::vec3 GetGroupRotation() { return rotation; }
	glm::vec3 GetGroupScale() { return scale; }
	string GetName() { return name; }
	string GetParentName() { return pName; }
	bool GetIsChild() { return isChild; }
	int GetParentType() { return parentType; }
	Mesh * GetMeshParent() { return myMeshParent; }
	MeshGroupClass * GetGroupParent() { return myGroupParent; }

	glm::vec3 GetParentPosOffset() const { return parentPosOffset; }
	void SetParentPosOffset(glm::vec3 offset);


};

#endif MESHGROUPCLASS_H