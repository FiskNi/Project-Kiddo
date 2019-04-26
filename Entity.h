#pragma once
#include "Headers.h"
#include "Mesh.h"

//============================================================================
//	- Entities
//	Entities are interactable objects in the world. More or less this includes 
//	everything we can see and define as objects in the scene.
//	This includes walls, boxes, buttons, etc.
//	An entity holds one mesh (currently) that defines the vertice data for renderering.
//	Every entity will have it's own AABB (hitbox) created for it based on the linked mesh.
//
//	*Note, This class should be changed to a virtual class where only the derived classes are usable
//
//	Collision (Bounding Box):
//	The boundingbox might need to be offset from the center of the mesh itself. When making calculations	
//
//	- Usage:
//	Don't. Use the derived classes 'Static' or 'Rigid'. Static meshes are unmoveable objects that
//	the player can collide with, examples are walls and ground. Rigids are moveable objects like													 		
//	boxes.
//	Recommended to create new derived classes for specific usage, these can additionally
//	be derived from the already derived classes. A 'Lever' class could for example be a derived 
//	'Static' class. Suggested to keep names consistent within entities (example "LeverEntity")
//============================================================================

class Entity
{
private:
	Mesh entityMesh;
	//glm::vec3 position;
	//glm::vec3 rotation;

	// The center is a vector to a location in the world
	glm::vec3 boundingBoxCenter;
	// The size is the half-size in each direction measured from the center
	//	x, y, z, format in worldspace (axis aligned)
	glm::vec3 boundingBoxSize;

	glm::vec3 savedPosition;

public:
	Entity(unsigned int i);
	Entity(vertex* vertArr, unsigned int nrOfVerticies);
	~Entity();

	void InitBoundingBox();

	bool CheckCollision(Entity collidingCube);

	void SetMaterialID(unsigned int materialID);
	void SetPosition(glm::vec3 newPos);

	void SetRotation(float x, float y, float z);
	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);

	void SaveCurrentPosition(glm::vec3 pos);
	void RestoreSavedPosition();
	void SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize); // Should be private maybe

	// Fixed return to be here
	Mesh GetMeshData() const;

	glm::vec3 GetPosition() const;
	glm::vec3 GetSavedPosition() const;
	glm::vec3 GetPositionBB() const;
	glm::vec3 GetBoundingBoxSize() const;

	float GetHitboxBottom() const;
	float GetHitboxTop() const;
};