#pragma once
#include "RigidEntity.h"
#include "Material.h"

class Item : public RigidEntity
{
private:

	int type;
	bool pickedUp = false;
public:
	Item(unsigned int i = 1) : RigidEntity(i) {};
	~Item() {};

	void SetItemType(int type) { this->type = type; }
	void SetPickedUp(bool tf) {
		if (tf) {
			this->SetPosition(glm::vec3(0, -30, 0));
			this->pickedUp = tf;
		}
	}

	bool GetPickedUp() { return this->pickedUp; }
	int GetItemType() { return this->type; }

};
