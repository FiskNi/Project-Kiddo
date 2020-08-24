#pragma once
#include "StaticEntity.h"

class PressurePlate : public StaticEntity
{

private:
	bool pressed;
	int linkID;

public:
	PressurePlate(Loader* inLoader, unsigned int index, unsigned int matID);
	PressurePlate(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen);
	~PressurePlate() {};

	bool isPressed() { return pressed; }
	int GetLinkID() { return linkID; }

	void setPressed(bool tf);
	void SetLink(int id);
	bool CheckLinkID(int id);
};