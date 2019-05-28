#pragma once
#include "Headers.h"
#include "Mesh.h"
#include "RigidEntity.h"
#include "Item.h"
#include "Collectible.h"
//============================================================================
//	- Character
//	The character as can be seen derives from a rigid entity.
//	This means any physic updates based on velocty etc, can easily be appplied
//	on the character as well as any moveable objects.
//============================================================================
class Character
	: public RigidEntity
{
private:
	const int itemCap = 4;
	//const int collCap = 8;

	int currentItem = 0;
	int nrOf = 0;
	std::vector<Item> items;
	std::vector<Collectible> collected;
	//ID, playerID = 1, Movable box = 2.
	int entityID;
	bool holdingObject;

	glm::vec3 inputVector;
	bool jumpSquat = false;

	glm::vec3 respawnPos;

	irrklang::ISoundEngine* soundEffectEngine;

public:
	Character();
	Character(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen);
	Character(Loader* inLoader, unsigned int index, unsigned int matID);
	~Character();
	
	void SetHoldingObject(bool holding);
	void SetEntityID(unsigned int id);
	void SetRespawn(glm::vec3 pos) { respawnPos = pos; }

	bool CheckInBound(Entity collidingCube);
	void Move(GLFWwindow *window);

	bool IsHoldingObject() { return holdingObject; }
	int GetEntityID() const { return entityID; }
	glm::vec3 GetInputVector() { return inputVector; }
	glm::vec3 GetRespawnPos() { return respawnPos; }
	Item GetCurrentItem() { return this->items[this->currentItem]; }
	std::vector<Collectible>& GetCollectedCollectibles();

	//void PickUpItem(Item* item);
	void PickUpCollectible(Collectible* coll);
	void SetCurrentItem(int i) { this->currentItem = i; }
	//Item* Upgrade();


};