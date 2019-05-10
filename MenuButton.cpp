#include "MenuButton.h"

MenuButton::MenuButton(Loader* inLoader, unsigned int index, unsigned int matID) : menuButtonMesh(inLoader->GetVerticies(index), inLoader->GetVertexCount(index)) 
{
	// Created a bounding box based on the entityMesh 
	LoaderMesh temp = inLoader->GetMesh(index);
	name = inLoader->GetMesh(index).name;

	position = glm::vec3(inLoader->GetMesh(index).translation[0], inLoader->GetMesh(index).translation[1], inLoader->GetMesh(index).translation[2]);
	rotation = glm::vec3(inLoader->GetMesh(index).rotation[0], inLoader->GetMesh(index).rotation[1], inLoader->GetMesh(index).rotation[2]);
	scale = glm::vec3(inLoader->GetMesh(index).scale[0], inLoader->GetMesh(index).scale[1], inLoader->GetMesh(index).scale[2]);

	for (int i = 0; i < menuButtonMesh.GetVertices().size(); i++)
	{
		//glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position);
		//entityMesh.ModifyVertices()[i].position = glm::vec3(translationMatrix * glm::vec4(entityMesh.GetVertices()[i].position, 1.0f));
	}
	InitBoundingBox();

	menuButtonMesh.setPosition(position);

	//SetMaterialID(matID);
}

MenuButton::~MenuButton() {

}

void MenuButton::InitBoundingBox()
{
	glm::vec3 min = menuButtonMesh.GetVertices()[0].position;
	glm::vec3 max = menuButtonMesh.GetVertices()[0].position;

	for (int i = 1; i < menuButtonMesh.GetVertices().size(); i++)
	{
		min.x = fminf(menuButtonMesh.GetVertices()[i].position.x, min.x);
		min.y = fminf(menuButtonMesh.GetVertices()[i].position.y, min.y);
		min.z = fminf(menuButtonMesh.GetVertices()[i].position.z, min.z);

		max.x = fmaxf(menuButtonMesh.GetVertices()[i].position.x, max.x);
		max.y = fmaxf(menuButtonMesh.GetVertices()[i].position.y, max.y);
		max.z = fmaxf(menuButtonMesh.GetVertices()[i].position.z, max.z);
	}

	glm::vec3 center = glm::vec3((min + max) * 0.5f);
	glm::vec3 halfSize = glm::vec3((max - min) * 0.5f);

	SetBoundingBox(center, halfSize);
}

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

void MenuButton::SetPosition(glm::vec3 newPos)
{
	menuButtonMesh.setPosition(newPos);

}

void MenuButton::SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize)
{
	boundingBoxSize = BBoxHalfSize;
	boundingBoxCenter = BBoxCenter;
}