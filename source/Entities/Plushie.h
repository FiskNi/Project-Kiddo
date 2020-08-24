#pragma once
#include "StaticEntity.h"
class Plushie :
	public StaticEntity
{
private:
	bool collected;

public:
	Plushie(Loader* inLoader, unsigned int index, unsigned int boxMatID, bool frozen);
	~Plushie();

	void SetCollected(bool col);
	bool IsCollected() { return collected; }
};

