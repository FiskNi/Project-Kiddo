#pragma once
#include "Headers.h"
#include "Mesh.h"

// Base class idea for Menu Buttons if we need to implement them ourselves
// Currently not used, but might be needed if we change our ideas.

class MenuButton {
private:
	Mesh menuButtonMesh;

	string name;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 boundingBoxCenter;
	glm::vec3 boundingBoxSize;
public:
	MenuButton(Loader* inLoader, unsigned int index, unsigned int matID);
	~MenuButton();
	void InitBoundingBox();

	void SetPosition(glm::vec3 newPos);

	void SetBoundingBox(glm::vec3 BBoxCenter, glm::vec3 BBoxHalfSize);


};

