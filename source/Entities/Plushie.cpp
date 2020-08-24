#include "Plushie.h"

Plushie::Plushie(Loader* inLoader, unsigned int index, unsigned int boxMatID, bool frozen) : StaticEntity(inLoader, index, boxMatID, frozen)
{
	collected = false;
}

Plushie::~Plushie()
{
}

void Plushie::SetCollected(bool col)
{
	collected = col;
}
