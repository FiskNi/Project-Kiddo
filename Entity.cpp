#include "Entity.h"


Entity::Entity()
{
	entityMesh.CreateCubeData();

	glm::vec3 startPos = glm::vec3(-4.0f, 0.0f, 0.0f);
	setPosition(startPos);
	boundingBoxSize = glm::vec3(0.5f, 0.5f, 0.5f);
}


Entity::~Entity()
{
}

Primitive Entity::getMeshData() const
{
	return entityMesh;
}

glm::vec3 Entity::getPosition() const
{
	return entityMesh.getPosition();
}

glm::vec3 Entity::getSize() const
{
	return boundingBoxSize;
}

void Entity::setPosition(glm::vec3 newPos)
{
	entityMesh.setPosition(newPos);
}

bool Entity::CheckCollision(Entity collidingCube)
{
	struct AABB
	{
		glm::vec3 position;
		glm::vec3 size;
	};

	AABB thisBoundingBox;
	thisBoundingBox.position = getPosition();
	thisBoundingBox.size = boundingBoxSize;

	AABB collidingBoundingBox;
	collidingBoundingBox.position = collidingCube.getPosition();
	collidingBoundingBox.size = collidingCube.getSize();

	glm::vec3 box1p1 = thisBoundingBox.position + thisBoundingBox.size;
	glm::vec3 box1p2 = thisBoundingBox.position - thisBoundingBox.size;
	glm::vec3 box1min = glm::vec3(fminf(box1p1.x, box1p2.x), fminf(box1p1.y, box1p2.y), fminf(box1p1.z, box1p2.z));
	glm::vec3 box1max = glm::vec3(fmaxf(box1p1.x, box1p2.x), fmaxf(box1p1.y, box1p2.y), fmaxf(box1p1.z, box1p2.z));

	glm::vec3 box2p1 = collidingBoundingBox.position + collidingBoundingBox.size;
	glm::vec3 box2p2 = collidingBoundingBox.position - collidingBoundingBox.size;
	glm::vec3 box2min = glm::vec3(fminf(box2p1.x, box2p2.x), fminf(box2p1.y, box2p2.y), fminf(box2p1.z, box2p2.z));
	glm::vec3 box2max = glm::vec3(fmaxf(box2p1.x, box2p2.x), fmaxf(box2p1.y, box2p2.y), fmaxf(box2p1.z, box2p2.z));

	if ((box1min.x <= box2max.x && box1max.x >= box2min.x) &&
		(box1min.y <= box2max.y && box1max.y >= box2min.y) &&
		(box1min.z <= box2max.z && box1max.z >= box2min.z))
	{
		return true;
	}

	return false;

}

