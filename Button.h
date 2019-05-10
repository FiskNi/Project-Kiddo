#pragma once
#include "StaticEntity.h"

class Button : public StaticEntity
{
private:
	bool pressed = false;
	int linkID = 0;

public:
	Button() : StaticEntity(2) {};
	~Button() {};

	bool isPressed() { return pressed; }
	int GetLinkID() { return linkID; }

	void setPressed(bool tf) { pressed = tf; }
	void SetLink(int id) { this->linkID = id; }
};
