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
	BridgeEntity(Vertex* vertArr, unsigned int vertexCount, unsigned int matID);
	BridgeEntity(Loader* vertArr, unsigned int index, unsigned int matID);
	BridgeEntity(Loader* vertArr, unsigned int index, unsigned int matID, bool frozen);
	~BridgeEntity();

	void SetRestPosition(glm::vec3 position);
	void SetRestPosition(float x, float y, float z);
	void SetExtendDistance(float d);
	void SetExtendingDir(int dir);
	void SetExtendingForwardX();
	void SetExtendingBackwardX();
	void SetExtendingForwardZ();
	void SetExtendingBackwardZ();
	void SetLinkID(int id);

	bool CheckLinkID(int id);
	int GetLinkID() { return linkID; }

	void Extend();
	void Retract();
	void Update(float deltaTime);
	
	bool getExtended() { return extended; }

};

