#include "MenuButton.h"

MenuButton::MenuButton(float offset, int textureID) 
{
	this->offset = offset;
	this->textureID = textureID;
	this->isNotButton = false;
	
	CreateButtonQuad();
	CalculateBoundingBox();
	//std::cout << "Button BBox MIN: " << cornerMin.x << "  " << cornerMin.y << std::endl;
	//std::cout << "Button BBox MAX: " << cornerMax.x << "  " << cornerMax.y << std::endl;
}

MenuButton::MenuButton(std::vector<ButtonVtx> vertices, int textureID, bool isNotButton)
{
	this->offset = 0;
	this->textureID = textureID;
	this->isNotButton = isNotButton;

	for (int i = 0; i < 6; i++) {
		buttonVertices.push_back(vertices[i]);
	}

	CalculateBoundingBox();
}

MenuButton::MenuButton(int minX, int minY, int maxX, int maxY, int textureID)
{
	this->offset = 0;
	this->textureID = textureID;
	this->isNotButton = false;

	CreateButtonQuad();

	cornerMin.x = minX;
	cornerMin.y = minY;
	cornerMax.x = maxX;
	cornerMax.y = maxY;

}

MenuButton::~MenuButton() {

}

//=============================================================
//	Creates a button quad, while adding the offset for the next button
//	This should stop the created buttons from overlapping
//=============================================================
void MenuButton::CreateButtonQuad()
{
	// Add an offset to the y coordinate in order to offset upcoming buttons once the shape has been defined
	//ButtonVtx buttVtxTemp[6] = {
	//	-0.3f,(0.0f - offset)	,0.0f,		0.0f, 0.0f,	// TOP		LEFT	// bot left?
	//	-0.3f,(+0.3f - offset)	,0.0f,		0.0f, 1.0f,	// BOTTOM	LEFT	// top left?
	//	+0.3f,(+0.3f - offset)	,0.0f,		1.0f, 1.0f,	// BOTTOM	RIGHT	// top right?
	//	-0.3f,(0.0f - offset)	,0.0f,		0.0f, 0.0f,	// TOP		LEFT	
	//	+0.3f,(+0.3f - offset)	,0.0f,		1.0f, 1.0f,	// BOTTOM	RIGHT
	//	+0.3f,(0.0f - offset)	,0.0f,		1.0f, 0.0f,	// TOP		RIGHT	// bot right?
	//};
	ButtonVtx buttVtxTemp[6] = {
	(-0.3f + offset), 0.0f		,0.0f,		0.0f, 0.0f,	// TOP		LEFT	// bot left?
	(-0.3f + offset), 0.3f		,0.0f,		0.0f, 1.0f,	// BOTTOM	LEFT	// top left?
	(+0.0f + offset), 0.3f		,0.0f,		1.0f, 1.0f,	// BOTTOM	RIGHT	// top right?
	(-0.3f + offset), 0.0f		,0.0f,		0.0f, 0.0f,	// TOP		LEFT	
	(+0.0f + offset), 0.3f		,0.0f,		1.0f, 1.0f,	// BOTTOM	RIGHT
	(+0.0f + offset), 0.0f		,0.0f,		1.0f, 0.0f,	// TOP		RIGHT	// bot right?
	};

	for (int i = 0; i < 6; i++) {
		buttonVertices.push_back(buttVtxTemp[i]);
	}

}

//=============================================================
//	Calculates the bounding box based on the vertices for the button
//=============================================================
void MenuButton::CalculateBoundingBox()
{
	// Calculates the top left and the bottom right corners to determine which vertices are the min and max corners
	int topLeftIdx = 0;
	int botRightIdx = 0;

	for (int i = 0; i < 6; i++) {
		if (buttonVertices[i].x <= buttonVertices[topLeftIdx].x && buttonVertices[i].y >= buttonVertices[topLeftIdx].y ) {
			topLeftIdx = i;
		}
		if (buttonVertices[i].x >= buttonVertices[botRightIdx].x && buttonVertices[i].y <= buttonVertices[botRightIdx].y) {
			botRightIdx = i;
		}
	}

	// Calculates the pixel position for the vertex coordinates created in -1 to 1 space
	// Y currently needs -20 as an offset in all cases, might need to be adjusted depending on the button offset in the menu?
	cornerMin.x = (WIDTH / 2) * (1 + buttonVertices[topLeftIdx].x);
	cornerMin.y = (HEIGHT / 2) * (1 - buttonVertices[topLeftIdx].y) - 20;

	cornerMax.x = (WIDTH / 2) * (1 + buttonVertices[botRightIdx].x);
	cornerMax.y = (HEIGHT / 2) * (1 - buttonVertices[botRightIdx].y) - 20;
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
