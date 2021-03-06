#pragma once
#include "StaticEntity.h"
#include "Headers.h"

class Collectible : public StaticEntity
{
private:
	bool collected = false;

	//Index determines which collectible it is, current goes from 0-7
	int index = -1;
public:
	Collectible(unsigned int i = 1) : StaticEntity(i) {};
	Collectible(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen);
	~Collectible() {};

	void SetCollected(bool tf) { this->collected = tf; }
	void SetIndex(int i) { this->index = i; }

	bool GetCollected() { return this->collected; }
	int GetIndex() { return this->index; }
};
