#pragma once
#include "StaticEntity.h"
class BridgeEntity :
	public StaticEntity
{
private:
	glm::vec3 restPosition;
	bool extended;


public:
	BridgeEntity(unsigned int i);
	BridgeEntity(vertex* vertArr, unsigned int nrOfVerticies);
	~BridgeEntity();

	void SetRestPosition(glm::vec3 position);

	void Extend();
	void Retract();
	void Update();
	
	bool getExtended() { return extended; }

};

