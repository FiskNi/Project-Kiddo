#pragma once
#include "Rendering/Mesh.h"

class Door : public StaticEntity
{
private:

public:
	Door(Loader* inLoader, int index, unsigned int matID) : StaticEntity(inLoader, index, matID){};
	~Door() {};

};