#pragma once
#include "StaticEntity.h"

class Collectible : public StaticEntity
{
private:
	bool collected = false;
	int index = -1;
public:
	Collectible(unsigned int i = 1) : StaticEntity(i) {};
	~Collectible() {};

	void SetCollected(bool tf) { this->collected = tf; }
	void SetIndex(int i) { this->index = i; }

	bool GetCollected() { return this->collected; }
	int GetIndex() { return this->index; }
};
