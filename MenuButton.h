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

	float offset;

	glm::vec3 cornerMin;
	glm::vec3 cornerMax;

public:
	MenuButton(float offset, int textureID);
	~MenuButton();
	void CreateButtonQuad();
	void CalculateBoundingBox();

	bool CheckInsideCollision(float xPos, float yPos);

	std::vector<ButtonVtx> GetButtonVertices() const { return buttonVertices; }
	glm::vec3 GetVertexPosition(int idx) const { return glm::vec3(buttonVertices[idx].vtxPos[0], buttonVertices[idx].vtxPos[1], buttonVertices[idx].vtxPos[2]); }
	// Hardcoded to 6 vertices, as a button will always consist of two quads.
	int GetSize() const { return sizeof(ButtonVtx) * 6; }
	int GetVertexCount() const { return 6; }
	int GetTextureID() const { return textureID; }

};

