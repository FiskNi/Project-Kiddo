#include "MenuButton.h"

MenuButton::MenuButton(float offset = 0, int textureID = 0) 
{
	this->offset = offset;
	this->textureID = textureID;	// Is this correct?
	
	CreateButtonQuad();
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
	ButtonVtx buttVtxTemp[6] = {
		-0.5f,(-0.5f + offset),0.0f,    0.0f, 0.0f,	// TOP		LEFT
		-0.5f,(+0.5f + offset),0.0f,	0.0f, 1.0f,	// BOTTOM	LEFT
		+0.5f,(+0.5f + offset),0.0f,	1.0f, 1.0f,	// BOTTOM	RIGHT
		-0.5f,(-0.5f + offset),0.0f,	0.0f, 0.0f,	// TOP		LEFT
		+0.5f,(+0.5f + offset),0.0f,	1.0f, 1.0f,	// BOTTOM	RIGHT
		+0.5f,(-0.5f + offset),0.0f,	1.0f, 0.0f,	// TOP		RIGHT
	};

	for (int i = 0; i < 6; i++) {
		//buttonVertices[i] = buttVtxTemp[i];
		buttonVertices.push_back(buttVtxTemp[i]);
	}

};

//void MenuButton::InitBoundingBox()
//{
//	glm::vec3 min = this->GetVertexPosition(0);
//	glm::vec3 max = menuButtonMesh.GetVertices()[0].position;
//
//	for (int i = 1; i < menuButtonMesh.GetVertices().size(); i++)
//	{
//		min.x = fminf(menuButtonMesh.GetVertices()[i].position.x, min.x);
//		min.y = fminf(menuButtonMesh.GetVertices()[i].position.y, min.y);
//		//min.z = fminf(menuButtonMesh.GetVertices()[i].position.z, min.z);
//
//		max.x = fmaxf(menuButtonMesh.GetVertices()[i].position.x, max.x);
//		max.y = fmaxf(menuButtonMesh.GetVertices()[i].position.y, max.y);
//		//max.z = fmaxf(menuButtonMesh.GetVertices()[i].position.z, max.z);
//	}
//
//	glm::vec3 center = glm::vec3((min + max) * 0.5f);
//	glm::vec3 halfSize = glm::vec3((max - min) * 0.5f);
//
//	SetBoundingBox(center, halfSize);
//}

//*** USE THIS BASE FOR MOUSE PICKING
//bool MenuButton::CheckInsideCollision(Entity AABB)
//{
//	if (AABB.GetPositionBB().x + AABB.boundingBoxSize.x < this->GetPositionBB().x + boundingBoxSize.x
//		&& AABB.GetPositionBB().x - AABB.boundingBoxSize.x > this->GetPositionBB().x - boundingBoxSize.x
//		&& AABB.GetPositionBB().y + AABB.boundingBoxSize.y < this->GetPositionBB().y + boundingBoxSize.y
//		&& AABB.GetPositionBB().y - AABB.boundingBoxSize.y > this->GetPositionBB().y - boundingBoxSize.y
//		&& AABB.GetPositionBB().z + AABB.boundingBoxSize.z < this->GetPositionBB().z + boundingBoxSize.z
//		&& AABB.GetPositionBB().z - AABB.boundingBoxSize.z > this->GetPositionBB().z - boundingBoxSize.z)
//	{
//		return true;
//	}
//	else return false;
//}

//void MenuButton::SetPosition(glm::vec3 newPos)
//{
//	menuButtonMesh.setPosition(newPos);
//
//}
//
//void MenuButton::SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize)
//{
//	boundingBoxSize = BBoxHalfSize;
//	boundingBoxCenter = BBoxCenter;
//}