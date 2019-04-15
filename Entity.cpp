#include "Entity.h"


Entity::Entity()
{
	entityMesh.CreateCubeData();

	glm::vec3 startPos = glm::vec3(-4.0f, 0.0f, 0.0f);
	setPosition(startPos);
}


Entity::~Entity()
{
}

Primitive Entity::getMeshData() const
{
	return entityMesh;
}

void Entity::setPosition(glm::vec3 newPos)
{
	entityMesh.setPosition(newPos);
}

