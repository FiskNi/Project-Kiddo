#pragma once
#include "StaticEntity.h"

class PressurePlate : public StaticEntity
{
private:
	glm::vec3 position;
	bool pressed = false;
	bool playerButton = false;

public:
	PressurePlate() : StaticEntity(2) {};
	~PressurePlate() {};
	void setPressed(bool tf) { pressed = tf; }
	void setPosition(glm::vec3 pos) { this->position = pos; }
	void setPlayerButton(bool tf) { this->playerButton = tf; }

	bool isPressed() { return this->pressed; }
	glm::vec3 getPosition() { return this->position; }
	bool isPlayerButton() { return this->playerButton; }



};
