#pragma once
#include "Headers.h"
#include "Primitive.h"

class Entity
{
private:
	Primitive entityMesh;

	glm::vec3 boundingBoxCenter;
	glm::vec3 boundingBoxSize;

	//ID, playerID = 1, Movable box = 2.
	unsigned int entityID; 
	float speed;

	bool collision;

public:
	Entity();
	~Entity();

	void InitBoundingBox();

	bool CheckCollision(Entity collidingCube);

	void setMaterialID(unsigned int materialID);
	void setPosition(glm::vec3 newPos);
	void setSpeed(float speed);
	void SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize); // Should be private maybe
	void setIndex(int index);

	void setCustomBBox(glm::vec3);

	unsigned int getEntityID() const;

	Primitive getMeshData() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSize() const;
	float getSpeed() const;
	int getIndex() const;


	glm::vec3 entMove(GLFWwindow* window, float dTime);
	glm::vec3 calcMovement(float moveX, float moveY, float moveZ, Primitive mesh);
};

}