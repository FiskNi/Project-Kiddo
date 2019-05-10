#pragma once
#include "StaticEntity.h"

class PressurePlate : public StaticEntity
{

private:
	bool pressed = false;
	int linkID;

public:
	PressurePlate() : StaticEntity(2) {};
	~PressurePlate() {};

	bool isPressed() { return pressed; }
	int GetLinkID() { return linkID; }

	void setPressed(bool tf);
	void SetLink(int id) { this->linkID = id; }

};