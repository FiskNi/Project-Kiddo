#pragma once
#include "RigidEntity.h"
#include "Rendering/Material.h"

class Item : public RigidEntity
{
private:

	ITEMTYPE type = NONE;
	bool pickedUp = false;
public:
	Item(unsigned int i = 1) : RigidEntity(i) {};
	~Item() {};

	void SetItemType(ITEMTYPE type) { this->type = type; }
	void SetPickedUp(bool tf) {
		if (tf) {
			this->SetPosition(glm::vec3(0, -30, 0));
			this->pickedUp = tf;
		}
	}

	bool GetPickedUp() { return this->pickedUp; }
	ITEMTYPE GetItemType() { return this->type; }

};
