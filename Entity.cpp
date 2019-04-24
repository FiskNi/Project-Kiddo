#include "Entity.h"


Entity::Entity(unsigned int i)
{
	// This is where the mesh data is created
	// Currently it creates either a primitive plane or cube
	// This will be changed for a imported mesh
	if (i == 0)
		entityMesh.CreatePlaneData();
	else
		entityMesh.CreateCubeData();

	// Starting position
	// Should be input with the constuctor and possibly required
	glm::vec3 startPos = glm::vec3(-4.0f, 0.0f, 0.0f);
	SetPosition(startPos);

	// For position calculations and vector math
	savedPosition = GetPosition();

	// Created a bounding box based on the entityMesh
	InitBoundingBox();

	// Entity ID for collision checks
	this->entityID = 2;
}

Entity::~Entity()
{

}

void Entity::InitBoundingBox()
{
	glm::vec3 min = entityMesh.getvertexPolygons()[0].position;
	glm::vec3 max = entityMesh.getvertexPolygons()[0].position;

	for (int i = 1; i < entityMesh.getvertexPolygons().size(); i++)
	{
		min.x = fminf(entityMesh.getvertexPolygons()[i].position.x, min.x);
		min.y = fminf(entityMesh.getvertexPolygons()[i].position.y, min.y);
		min.z = fminf(entityMesh.getvertexPolygons()[i].position.z, min.z);

		max.x = fmaxf(entityMesh.getvertexPolygons()[i].position.x, max.x);
		max.y = fmaxf(entityMesh.getvertexPolygons()[i].position.y, max.y);
		max.z = fmaxf(entityMesh.getvertexPolygons()[i].position.z, max.z);
	}

	glm::vec3 center = glm::vec3((min + max) * 0.5f);
	glm::vec3 halfSize = glm::vec3((max - min) * 0.5f);

	SetBoundingBox(center, halfSize);
}

bool Entity::CheckCollision(Entity collidingCube)
{
	struct AABB
	{
		glm::vec3 position;
		glm::vec3 size;
	};

	AABB thisBoundingBox;
	thisBoundingBox.position = GetPosition() + boundingBoxCenter;
	thisBoundingBox.size = boundingBoxSize;

	AABB collidingBoundingBox;
	collidingBoundingBox.position = collidingCube.GetPosition();
	collidingBoundingBox.size = collidingCube.GetBoundingBoxSize();

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

void Entity::SetMaterialID(unsigned int id)
{
	entityMesh.setMaterial(id);
}

void Entity::SetPosition(glm::vec3 newPos)
{
	entityMesh.setPosition(newPos);
}

void Entity::SaveCurrentPosition(glm::vec3 pos)
{
	savedPosition = pos;
}

void Entity::RestoreSavedPosition()
{
	SetPosition(savedPosition);
}

void Entity::SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize)
{
	this->boundingBoxSize = BBoxHalfSize;
	this->boundingBoxCenter = BBoxCenter;
}

unsigned int Entity::getEntityID() const
{
	return this->entityID;
}

Primitive Entity::GetMeshData() const
{
	return entityMesh;
}

glm::vec3 Entity::GetPosition() const
{
	return entityMesh.getPosition();
}

glm::vec3 Entity::GetSavedPosition() const
{
	return savedPosition;
}

glm::vec3 Entity::GetPositionBB() const
{
	return GetPosition() + boundingBoxCenter;
}

glm::vec3 Entity::GetBoundingBoxSize() const
{
	return boundingBoxSize;
}

float Entity::GetHitboxBottom() const
{
	return GetPosition().y - boundingBoxSize.y;
}
