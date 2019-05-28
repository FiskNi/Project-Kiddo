#include "ColPlane.h"

ColPlane::ColPlane(Loader * inLoader, int index, unsigned int matID) : StaticEntity(inLoader, index, matID)
{
	origVertPos.reserve(inLoader->GetVertexCount(index));
	for (int i = 0; i < origVertPos.capacity(); i++)
	{
		Vertex vertexData = inLoader->GetVerticies(index)[i];
		vertexPolygon newVertex;
		newVertex.position = glm::vec3(vertexData.position[0], vertexData.position[1], vertexData.position[2]);
		newVertex.uv = glm::vec2(vertexData.uv[0], vertexData.uv[1]);
		newVertex.normals = glm::vec3(vertexData.normal[0], vertexData.normal[1], vertexData.normal[2]);
		newVertex.tangent = glm::vec3(vertexData.tangent[0], vertexData.tangent[1], vertexData.tangent[2]);
		newVertex.bitangent = glm::vec3(vertexData.bitangent[0], vertexData.bitangent[1], vertexData.bitangent[2]);

		
		origVertPos.push_back(newVertex);
	}
	origPos = GetPosition();
	glm::vec3 temp = GetPositionBB();


	//Makes the plane "invisible". *Ahem* YEET!!
	//SetPosition(glm::vec3(999,999,999));

	setPositionBBOffset(temp);

	switchTriangle = false;
}

ColPlane::~ColPlane()
{

}

float ColPlane::GetGroundHeight(glm::vec3 playerPos)
{
	float localX = playerPos.x - origPos.x;
	float localZ = playerPos.y - origPos.z;

	//Calculating the rotated vertexpoints
	glm::vec3 pointA;
	glm::vec3 pointB;
	glm::vec3 pointC;

	//Now let's make this not hardcoded: 
	//1. Lowest points are always start points, the first one is for the "lower" tris
	//The second (else) is start point for the "higher" triangle |\|(LikeDis)
	//2. Second point second bottom for first tris, top left for second tris
	//3. Third point is highest right point for bottom tris and also for top tris.
	//EDIT: THAT THEY'RE HARD-CODED DOESN'T SEEM TO MATTER AS MAYA ALWAYS CREATES VERTECIES IN THE SAME ORDER FOR PLANES
	//Leaving this comment until testing with other people's computers and maya can confirm the above

	//All math in this is for if the planes actually have rotations. 
	//If they never will due to freeze-workflow we could technically just assign the point vertex to be the point directly.
	pointA = origPos + (GetMeshDataPointer()->GetRotation() * (origVertPos[0].position - origPos));
	pointB = origPos + (GetMeshDataPointer()->GetRotation() * (origVertPos[1].position - origPos));
	pointC = origPos + (GetMeshDataPointer()->GetRotation() * (origVertPos[2].position - origPos));


	float ground = -1;

	float point = CalculateBarycentricCoords(pointA, pointB, pointC, playerPos);
	if (point != -1)
	{
		ground = point;
	}
	else
	{
		pointA = origPos + (GetMeshDataPointer()->GetRotation() * (origVertPos[4].position - origPos));
		pointB = origPos + (GetMeshDataPointer()->GetRotation() * (origVertPos[5].position - origPos));
		pointC = origPos + (GetMeshDataPointer()->GetRotation() * (origVertPos[3].position - origPos));

		point = CalculateBarycentricCoords(pointA, pointB, pointC, playerPos);

		if (point != -1)
		{
			ground = point;
		}
	}
	return ground;
}

float ColPlane::CalculateBarycentricCoords(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 pos)
{
	glm::vec2 v0 = glm::vec2(p2.x, p2.z) - glm::vec2(p1.x, p1.z), v1 = glm::vec2(p3.x, p3.z) - glm::vec2(p1.x, p1.z), v2 = glm::vec2(pos.x, pos.z) - glm::vec2(p1.x, p1.z);

	const float denominator = v0.x * v1.y - v1.x * v0.y;
	float v = (v2.x * v1.y - v1.x * v2.y) / denominator;
	float w = (v0.x * v2.y - v2.x * v0.y) / denominator;
	float u = 1.0f - v - w;

	if (((0 <= u) && (u <= 1)) == false || ((0 <= v) && (v <= 1)) == false || ((0 <= w) && (w <= 1)) == false)
	{
		return -1.0;
	}

	float heightReturn = u*p1.y + v * p2.y + w * p3.y;


	return heightReturn;

}
