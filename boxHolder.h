#pragma once

#include "StaticEntity.h"
#include "Material.h"

class boxHolder : public StaticEntity
{
private:
	glm::vec3 origPos;
	glm::vec3 origBBPos;
	bool visible;

	Mesh aMesh;
	
public:
	boxHolder(unsigned int i);
	boxHolder(Loader* inLoader, unsigned int index, unsigned int holderMatID, unsigned int boxMatID);
	boxHolder(Loader* inLoader, unsigned int index, unsigned int holderMatID, unsigned int boxMatID, bool frozen);
	~boxHolder();

	void puntBox();
	void boxReturn();

	void setVisible(bool aBool);
	bool getVisible() const { return this->visible; }
	void SetBoxHolderPosition(glm::vec3 pos);
	Mesh GetHolderMeshData();


};
