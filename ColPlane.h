#pragma once
#include "StaticEntity.h"

class ColPlane :public StaticEntity
{
private:
	std::vector<vertexPolygon> origVertPos;
	bool switchTriangle;
	glm::vec3 origPos;


public:
	ColPlane(Loader* inLoader, int index, unsigned int matID);
	~ColPlane();

	float GetGroundHeight(glm::vec3 playerPos);

	float CalculateBarycentricCoords(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 pos);
};
