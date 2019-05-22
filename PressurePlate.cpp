#include "PressurePlate.h"


PressurePlate::PressurePlate(Loader* inLoader, unsigned int index, unsigned int matID) : StaticEntity(inLoader, index, matID)
{
	pressed = false;
	linkID = -999;
}

PressurePlate::PressurePlate(Loader * inLoader, unsigned int index, unsigned int matID, bool frozen) : StaticEntity(inLoader, index, matID, frozen)
{
	pressed = false;
	linkID = -999;
}

void PressurePlate::setPressed(bool prssd)
{
	pressed = prssd;
}

void PressurePlate::SetLink(int id)
{
	linkID = id;
}

bool PressurePlate::CheckLinkID(int id)
{
	if (id == linkID)
		return true;

	return false;
}