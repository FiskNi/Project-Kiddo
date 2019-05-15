#pragma once
#include "Headers.h"


// Base class idea for Menu Buttons if we need to implement them ourselves
// Currently not used, but might be needed if we change our ideas.

struct ButtonVtx {
	float vtxPos[3];
	float uvPos[2];
};

class MenuButton {
private:
	int textureID;

	//string name;

	std::vector<ButtonVtx> buttonVertices;
	//ButtonVtx buttonVertices[6];

	float offset;

	//glm::vec3 position;
	//glm::vec3 rotation;
	//glm::vec3 scale;

	//glm::vec3 boundingBoxCenter;
	//glm::vec3 boundingBoxSize;
public:
	MenuButton(float offset, int textureID);
	~MenuButton();
	void CreateButtonQuad();

	std::vector<ButtonVtx> GetButtonVertices() const { return buttonVertices; }
	glm::vec3 GetVertexPosition(int idx) const { return glm::vec3(buttonVertices[idx].vtxPos[0], buttonVertices[idx].vtxPos[1], buttonVertices[idx].vtxPos[2]); }
	// Hardcoded to 6 vertices, as a button will always consist of two quads.
	int GetSize() const { return sizeof(ButtonVtx) * 6; }
	int GetVertexCount() const { return 6; }
	//void InitBoundingBox();

	//void SetPosition(glm::vec3 newPos);

	//void SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize);


};

