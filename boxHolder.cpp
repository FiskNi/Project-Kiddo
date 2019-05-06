#include "boxHolder.h"

boxHolder::boxHolder(unsigned int i) : StaticEntity (i)
{
	this->visible = false;
	SetPosition(glm::vec3(5, -1, 0));
	origPos = GetPosition();
	origBBPos = GetPositionBB();
	setPositionBBOffset(GetPositionBB());
}

boxHolder::boxHolder(Vertex * vertArr, unsigned int nrOfVerticies, unsigned int holderMatID, unsigned int boxMatID) : StaticEntity (vertArr, nrOfVerticies,boxMatID)
{
	this->visible = false;
	this->aMesh.CreatePlateData();
	//SetPosition(glm::vec3(5, -1, 0));
	origPos = GetPosition();
	origBBPos = GetPositionBB();
	setPositionBBOffset(origBBPos);


	aMesh.setPosition(origPos);
	aMesh.setMaterial(holderMatID);
}

boxHolder::~boxHolder()
{
}

void boxHolder::puntBox()
{
	SetPosition(glm::vec3(999, 0, 0));
}

void boxHolder::boxReturn()
{
	SetPosition(this->origPos);
}

void boxHolder::setVisible(bool aBool)
{
	this->visible = aBool;
}

void boxHolder::SetBoxHolderPosition(glm::vec3 pos)
{
	this->origPos = pos;
	SetPosition(pos);
	setPositionBBOffset(glm::vec3(0, 0, 0));
	setPositionBBOffset(GetPositionBB());
	aMesh.setPosition(origPos);
}

Mesh boxHolder::GetHolderMeshData()
{
	return this->aMesh;
}


