#pragma once
#include "StaticEntity.h"

class Button : public StaticEntity
{
private:
	bool pressed = false;
	int linkID = 0;

public:
	Button(Loader * inLoader, unsigned int index, unsigned int matID);
	Button(Loader * inLoader, unsigned int index, unsigned int matID, bool frozen);
	~Button() {};

	bool isPressed() { return pressed; }
	int GetLinkID() { return linkID; }

	void SetPressed(bool prssd);
	void SetLink(int id);
};
