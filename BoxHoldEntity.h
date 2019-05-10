#pragma once
#include "StaticEntity.h"
using namespace glm;
class BoxHoldEntity : public StaticEntity
{
private:
	vec3 pos;
	bool holdsBox = false; //set to false

public:
	BoxHoldEntity(int i) : StaticEntity(i){};
	BoxHoldEntity(vec3 pos) : StaticEntity(1) { this->pos = pos; }
	~BoxHoldEntity() {};

	// S E T T E R S
	void SetPosition(vec3 pos) { this->pos = pos; }
	void setHold(bool tf) { this->holdsBox = tf; }
	//G E T T E R S 
	vec3 getPosition() const{ return this->pos; }
	bool holdingBox() const { return this->holdsBox; }

};
