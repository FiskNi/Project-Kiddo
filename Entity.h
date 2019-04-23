#pragma once
#include "Headers.h"
#include "Primitive.h"

class Entity
{
private:
	Primitive entityMesh;
	glm::vec3 boundingBoxSize;

	//ID, playerID = 1, Movable box = 2.
	unsigned int entityID; 
	bool collision;

public:
	Entity();
	~Entity();

	bool CheckCollision(Entity collidingCube);

	void setMaterialID(unsigned int materialID);
	void setPosition(glm::vec3 newPos);
	void setIndex(int index);

	void setCustomBBox(glm::vec3);

	unsigned int getEntityID() const;

	Primitive getMeshData() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSize() const;
	int getIndex() const;


	glm::vec3 entMove(GLFWwindow* window, float dTime);
	glm::vec3 calcMovement(float moveX, float moveY, float moveZ, Primitive mesh);
};

