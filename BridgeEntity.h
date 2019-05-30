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
	bool isButton;

	int linkID;


public:
	BridgeEntity(unsigned int i);
	BridgeEntity(Loader* inLoader, unsigned int index, unsigned int matID);
	BridgeEntity(Loader* inLoader, unsigned int index, unsigned int matID, bool frozen, bool button);
	~BridgeEntity();

	void SetRestPosition(glm::vec3 position);
	void SetRestPosition(float x, float y, float z);
	void SetExtendDistance(float d);
	void SetExtendingDir(int dir);
	void SetLinkID(int id);

	bool CheckLinkID(int id);
	int GetLinkID() { return linkID; }

	void Extend();
	void Retract();
	void Update(float deltaTime);
	bool GetIsButton() { return isButton; }
	
	bool GetExtending() { return this->extending; }
	bool GetExtended() { return this->extended; }

};

