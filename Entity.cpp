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

	// Created a bounding box based on the entityMesh
	InitBoundingBox();
}

Entity::Entity(vertex* vertArr, unsigned int nrOfVerticies) : entityMesh(vertArr, nrOfVerticies)
{
	// Created a bounding box based on the entityMesh
	InitBoundingBox();

	// Scuffed solution for fixing the mesh center to be the center of the boundingbox instead
	// This should in theory also cause the boundingbox center to always be at 0,0,0 local
	glm::vec3 worldPosition = boundingBoxCenter;
	for (int i = 0; i < entityMesh.GetVertices().size(); i++)
	{
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -boundingBoxCenter);
		entityMesh.ModifyVertices()[i].position = glm::vec3(translationMatrix * glm::vec4(entityMesh.GetVertices()[i].position, 1.0f));
		
	}
	InitBoundingBox();
	entityMesh.setPosition(worldPosition);
}

Entity::~Entity()
{

}

void Entity::InitBoundingBox()
{
	glm::vec3 min = entityMesh.GetVertices()[0].position;
	glm::vec3 max = entityMesh.GetVertices()[0].position;

	for (int i = 1; i < entityMesh.GetVertices().size(); i++)
	{
		min.x = fminf(entityMesh.GetVertices()[i].position.x, min.x);
		min.y = fminf(entityMesh.GetVertices()[i].position.y, min.y);
		min.z = fminf(entityMesh.GetVertices()[i].position.z, min.z);

		max.x = fmaxf(entityMesh.GetVertices()[i].position.x, max.x);
		max.y = fmaxf(entityMesh.GetVertices()[i].position.y, max.y);
		max.z = fmaxf(entityMesh.GetVertices()[i].position.z, max.z);
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
	thisBoundingBox.position = GetPositionBB();
	thisBoundingBox.size = boundingBoxSize;

	AABB collidingBoundingBox;
	collidingBoundingBox.position = collidingCube.GetPositionBB();
	collidingBoundingBox.size = collidingCube.GetHitboxSize();

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

void Entity::SetPositionY(float y)
{
	entityMesh.setPositionY(y);
}

void Entity::OffsetPosition(glm::vec3 offset)
{
	entityMesh.setPosition(entityMesh.GetPosition() + offset);
}

void Entity::OffsetPosition(float x, float y, float z)
{
	entityMesh.setPosition(entityMesh.GetPosition() + glm::vec3(x, y, z));
}

void Entity::OffsetPositionX(float x)
{
	entityMesh.setPosition(glm::vec3(entityMesh.GetPosition().x + x, entityMesh.GetPosition().y, entityMesh.GetPosition().z));
}

void Entity::OffsetPositionY(float y)
{
	entityMesh.setPosition(glm::vec3(entityMesh.GetPosition().x, entityMesh.GetPosition().y + y, entityMesh.GetPosition().z));
}

void Entity::OffsetPositionZ(float z)
{
	entityMesh.setPosition(glm::vec3(entityMesh.GetPosition().x, entityMesh.GetPosition().y, entityMesh.GetPosition().z + z));
}

void Entity::SetRotation(float x, float y, float z)
{
	entityMesh.SetRotation(x, y, z);
}

void Entity::SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize)
{
	boundingBoxSize = BBoxHalfSize;
	boundingBoxCenter = BBoxCenter;
}
