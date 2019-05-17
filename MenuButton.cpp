#include "MenuButton.h"

MenuButton::MenuButton(float offset, int textureID) 
{
	this->offset = offset;
	this->textureID = textureID;
	
	CreateButtonQuad();
	CalculateBoundingBox();
	std::cout << "Button BBox MIN: " << cornerMin.x << "  " << cornerMin.y << std::endl;
	std::cout << "Button BBox MAX: " << cornerMax.x << "  " << cornerMax.y << std::endl;
}

MenuButton::~MenuButton() 
{

}

//=============================================================
//	Creates a button quad, while adding the offset for the next button
//	This should stop the created buttons from overlapping
//=============================================================
void MenuButton::CreateButtonQuad()
{
	// Add an offset to the y coordinate in order to offset upcoming buttons once the shape has been defined
	ButtonVtx buttVtxTemp[6] = 
	{
		-0.3f,(0.2f - offset)	,0.0f,		0.0f, 0.0f,	// TOP		LEFT	// bot left?
		-0.3f,(+0.5f - offset)	,0.0f,		0.0f, 1.0f,	// BOTTOM	LEFT	// top left?
		+0.3f,(+0.5f - offset)	,0.0f,		1.0f, 1.0f,	// BOTTOM	RIGHT	// top right?
		-0.3f,(0.2f - offset)	,0.0f,		0.0f, 0.0f,	// TOP		LEFT	
		+0.3f,(+0.5f - offset)	,0.0f,		1.0f, 1.0f,	// BOTTOM	RIGHT
		+0.3f,(0.2f - offset)	,0.0f,		1.0f, 0.0f,	// TOP		RIGHT	// bot right?
	};

	for (int i = 0; i < 6; i++) 
	{
		buttonVertices.push_back(buttVtxTemp[i]);
	}

}

void MenuButton::CalculateBoundingBox()
{
	// Calculates the pixel position for the vertex coordinates created in -1 to 1 space
	// Y currently needs -20 as an offset in all cases, might need to be adjusted depending on the button offset in the menu?
	cornerMin.x = (WIDTH / 2) * (1 + buttonVertices[1].vtxPos[0]);
	cornerMin.y = (HEIGHT / 2) * (1 - buttonVertices[1].vtxPos[1]) - 20;

	cornerMax.x = (WIDTH / 2) * (1 + buttonVertices[5].vtxPos[0]);
	cornerMax.y = (HEIGHT / 2) * (1 - buttonVertices[5].vtxPos[1]) - 20;
}

//*** USE THIS BASE FOR MOUSE PICKING
bool MenuButton::CheckInsideCollision(float xPos, float yPos)
{
	if (xPos < cornerMax.x && xPos > cornerMin.x && yPos < cornerMax.y && yPos > cornerMin.y)
	{
		return true;
	}
	else return false;
}
