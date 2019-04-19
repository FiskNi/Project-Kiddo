#include "puzzleNode.h"



puzzleNode::puzzleNode()
{
	setCustomBBox(glm::vec3(0.7f, 0.7f, 0.7f));
}


puzzleNode::~puzzleNode()
{
}

bool puzzleNode::checkInside(Entity collidingCube)
{
	struct AABB
	{
		glm::vec3 position;
		glm::vec3 size;
	};

	AABB thisBoundingBox;
	thisBoundingBox.position = getPosition();
	thisBoundingBox.size = getSize();

	AABB otherBoundingBox;
	otherBoundingBox.position = collidingCube.getPosition();
	otherBoundingBox.size = collidingCube.getSize();

	glm::vec3 box1p1 = thisBoundingBox.position + thisBoundingBox.size;
	glm::vec3 box1p2 = thisBoundingBox.position - thisBoundingBox.size;
	
	glm::vec3 box1min = glm::vec3(fminf(box1p1.x, box1p2.x), fminf(box1p1.y, box1p2.y), 
		fminf(box1p1.z, box1p2.z));
	glm::vec3 box1max = glm::vec3(fmaxf(box1p1.x, box1p2.x), fmaxf(box1p1.y, box1p2.y), 
		fmaxf(box1p1.z, box1p2.z));

	glm::vec3 box2p1 = otherBoundingBox.position + otherBoundingBox.size;
	glm::vec3 box2p2 = otherBoundingBox.position - otherBoundingBox.size;
	
	glm::vec3 box2min = glm::vec3(fminf(box2p1.x, box2p2.x), fminf(box2p1.y, box2p2.y),
		fminf(box2p1.z, box2p2.z));
	glm::vec3 box2max = glm::vec3(fmaxf(box2p1.x, box2p2.x));

	return true;
}

