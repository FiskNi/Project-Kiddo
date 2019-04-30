#pragma once
#include "StaticEntity.h"

class Button : public StaticEntity
{
private:
	bool pressed = false;

public:
	Button() : StaticEntity(2) {};
	~Button() {};

	bool isPressed() { return pressed; }

	void setPressed(bool tf) { pressed = tf; }
};
