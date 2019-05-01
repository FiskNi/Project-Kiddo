#pragma once
#include "StaticEntity.h"

class PressurePlate : public StaticEntity
{

private:
	bool pressed = false;

public:
	PressurePlate() : StaticEntity(2) {};
	~PressurePlate() {};

	bool isPressed() { return pressed; }

	void setPressed(bool tf) { pressed = tf; }
	
};