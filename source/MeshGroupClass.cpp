#include "MeshGroupClass.h"

MeshGroupClass::MeshGroupClass()
{
	this->position = glm::vec3(0, 0, 0);
	this->rotation = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(0, 0, 0);

	this->name = "Unasigned";
	this->pName = "Unasigned Parent";

	parentPosOffset = glm::vec3(0, 0, 0);
	isChild = false;
	parentType = -1;

	myGroupParent = nullptr;
	myMeshParent = nullptr;
}

MeshGroupClass::MeshGroupClass(Loader * inLoader, int index)
{
	position = glm::vec3(inLoader->GetMeshGroup(index).translation[0], 
		inLoader->GetMeshGroup(index).translation[1], inLoader->GetMeshGroup(index).translation[2]);

	rotation = glm::vec3(inLoader->GetMeshGroup(index).rotation[0],
		inLoader->GetMeshGroup(index).rotation[1], inLoader->GetMeshGroup(index).rotation[2]);

	scale = glm::vec3(inLoader->GetMeshGroup(index).scale[0],
		inLoader->GetMeshGroup(index).scale[1], inLoader->GetMeshGroup(index).scale[2]);

	name = inLoader->GetMeshGroup(index).groupName;

	if (inLoader->GetMeshGroup(index).isChild == true && inLoader->GetMeshGroup(index).parentType != -1)
	{
		pName = inLoader->GetMeshGroup(index).parentName;
		isChild = true;
	}
	else
	{
		pName = "NO MESHGROUP PARENT FOUND";
		isChild = false;
	}
	parentPosOffset = glm::vec3(0, 0, 0);
	parentType = inLoader->GetMeshGroup(index).parentType;

	myGroupParent = nullptr;
	myMeshParent = nullptr;

}

MeshGroupClass::~MeshGroupClass()
{
	//if (myGroupParent)
	//	delete myGroupParent;
	//if (myMeshParent)
	//	delete myMeshParent;
}

void MeshGroupClass::SetPosition(glm::vec3 newPos)
{
	position = newPos;
}

void MeshGroupClass::SetRotation(glm::vec3 newRot)
{
	rotation = newRot;
}

void MeshGroupClass::SetScale(glm::vec3 newScale)
{
	scale = newScale;
}

void MeshGroupClass::SetMeshParent(Mesh * parent)
{
	myMeshParent = parent;
}

void MeshGroupClass::SetGroupParent(MeshGroupClass * parent)
{
	myGroupParent = parent;
}

void MeshGroupClass::SetParentPosOffset(glm::vec3 offset)
{
	parentPosOffset = offset;
}
