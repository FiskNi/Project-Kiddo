#include "Button.h"



Button::Button(Loader * inLoader, unsigned int index, unsigned int matID) : StaticEntity(inLoader, index, matID)
{
	pressed = false;
	linkID = -999;
}

void Button::SetPressed(bool prssd)
{
	pressed = prssd;
}

void Button::SetLink(int id)
{
	linkID = id;
}
