#pragma once 
#include "Headers.h" 
#include "Rendering/Mesh.h" 

//============================================================================ 
//	- Entities 
//	Entities are interactable objects in the world. More or less this includes  
//	everything we can see and define as objects in the scene. 
//	This includes walls, boxes, buttons, etc. 
//	An entity holds one mesh (currently) that defines the vertice data for renderering. 
//	Every entity will have it's own AABB (hitbox) created for it based on the linked mesh. 
// 
//	*Note, This class should be changed to a abstract class where only the derived classes are usable. 
// 
//	- Usage: 
//	Don't. Use the derived classes 'Static' or 'Rigid'. Static meshes are unmoveable objects that 
//	the player can collide with, examples are walls and ground. Rigids are moveable objects like													 		 
//	boxes. 
//	Recommended to create new derived classes for specific usage, these can additionally 
//	be derived from the already derived classes. A 'Lever' class could for example be a derived  
//	'Static' class. Suggested to keep names consistent within entities (example "LeverEntity"). 
//============================================================================ 

class Entity
{
private:
	Mesh entityMesh;
	string name;
	string pName;

	// The center is a vector to a location in the world 
	glm::vec3 boundingBoxCenter;
	// The size is the half-size in each direction measured from the center 
	//	x, y, z, format in worldspace (axis aligned) 
	glm::vec3 boundingBoxSize;

public:
	Entity(unsigned int i);
	Entity(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen);
	Entity(Loader* inLoader, unsigned int index, unsigned int matID);
	~Entity();

	void InitBoundingBox();

	bool CheckCollision(const Entity& collidingCube);
	bool CheckHolderCollision(const Entity& collidingCube);
	bool CheckInsideCollision(const Entity& AABB);

	void SetMaterialID(unsigned int materialID);
	void SetPosition(glm::vec3 newPos);
	void SetPosition(float xyz[]);
	void SetPosition(float x, float y, float z);
	void SetPositionY(float y);
	void SetPositionX(float x);
	void SetPositionZ(float z);


	void OffsetPosition(glm::vec3 offset);
	void OffsetPosition(float x, float y, float z);
	void OffsetPositionX(float x);
	void OffsetPositionY(float y);
	void OffsetPositionZ(float z);

	void SetRotation(float x, float y, float z);
	void SetRotation(glm::vec3 newRot);
	void SetRotation(glm::quat newRot);
	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);

	void SetScale(float x, float y, float z);
	void SetScale(glm::vec3 newSca);
	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScaleZ(float z);

	void setPositionBBOffset(glm::vec3 newPos);

	void SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize); // Should be private maybe 
	void SetBoundingBox(glm::vec3 size);
	void scaleBB(float x);
	void scaleBBX(float x);
	void scaleBBZ(float z);
	void scaleBBY(float y);
	void setBBY(float y);

	// Fixed return to be here 
	Mesh& GetMeshData()						{ return entityMesh; }

	Mesh* GetMeshDataPointer()				{ return &entityMesh; }

	glm::vec3 GetPosition() const			{ return entityMesh.GetPosition(); }
	glm::vec3 GetPositionBB() const			{ return GetPosition() + boundingBoxCenter; }
	glm::vec3 GetHitboxSize() const			{ return boundingBoxSize; }
	glm::vec3 GetHitboxOffset() const		{ return boundingBoxCenter; }
	float GetHitboxBottom() const			{ return GetPosition().y - boundingBoxSize.y + boundingBoxCenter.y; }
	//float GetHitboxBottom() const			{ return boundingBoxCenter.y + boundingBoxSize.y; }
	float GetHitboxTop() const				{ return GetPosition().y + boundingBoxSize.y + boundingBoxCenter.y; }
	//float GetHitboxTop() const			{ return boundingBoxCenter.yboundingBoxSize.y; }

	float GetHitboxBottomOffsetBB() const	{ return GetHitboxOffset().y - boundingBoxSize.y; }
	float GetHitboxTopOffsetBB() const		{ return GetHitboxOffset().y + boundingBoxSize.y; }



};