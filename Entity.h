#pragma once
#include "Headers.h"
#include "Primitive.h"

class Entity
{
private:
	Primitive entityMesh;

	glm::vec3 boundingBoxCenter;
	glm::vec3 boundingBoxSize;

	glm::vec3 savedPosition;

	float speed;
	bool collision;

public:
	Entity(unsigned int i);
	~Entity();

	void InitBoundingBox();

	bool CheckCollision(Entity collidingCube);

	void SetMaterialID(unsigned int materialID);
	void SetPosition(glm::vec3 newPos);
	void SaveCurrentPosition(glm::vec3 pos);
	void RestoreSavedPosition();
	void SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize); // Should be private maybe

	Primitive GetMeshData() const;
	glm::vec3 GetPosition() const;
	glm::vec3 GetSavedPosition() const;
	glm::vec3 GetBoundingBoxSize() const;

	float GetBottom() const;

};