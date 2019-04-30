#pragma once
#include "StaticEntity.h"
class BridgeEntity :
	public StaticEntity
{
private:
	glm::vec3 restPosition;
	float extendDistance;
	glm::vec3 extendDirection;
	bool extended;
	bool extending;

	int linkID;


public:
	BridgeEntity(unsigned int i);
	BridgeEntity(vertex* vertArr, unsigned int nrOfVerticies);
	~BridgeEntity();

	void SetRestPosition(glm::vec3 position);
	void SetExtendDistance(float d);
	void SetExtendingForwardX();
	void SetExtendingBackwardX();
	void SetExtendingForwardZ();
	void SetExtendingBackwardZ();

	bool CheckLinkID(int id);

	void Extend();
	void Retract();
	void Update(float deltaTime);
	
	bool getExtended() { return extended; }

};

