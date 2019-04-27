#include "Mesh.h"

Mesh::Mesh(vertex* vertArr, unsigned int nrOfVerticies)
{
	this->worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->worldRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	ImportMesh(vertArr, nrOfVerticies);
}

Mesh::Mesh()
{
	this->worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->worldRotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

Mesh::~Mesh()
{

}

//=============================================================
//	Creates a hardcoded cube primitive and binds it into a VAO buffer
//=============================================================
void Mesh::CreateCubeData()
{
	// 36 hardcoded vertices representing a cube
	vertexPolygon cubeVertex;

	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(cubeVertex);

	CalculateTangents();
}

//=============================================================
//	Creates a hardcoded plane primitive and binds it into a VAO buffer
//=============================================================
void Mesh::CreatePlaneData()
{
	//// 6 hardcoded vertices representing a plane
	vertexPolygon planeVertex;
	planeVertex.position = glm::vec3(-10.0f, 0.0f, -10.0f);
	planeVertex.uv = glm::vec2(0.0f, 5.0f);
	planeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	//planeVertex.tangent = glm::vec3(0.0f);
	//planeVertex.bitangent = glm::vec3(0.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(10.0f, 0.0f, -10.0f);
	planeVertex.uv = glm::vec2(5.0f, 5.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(10.0f, 0.0f, 10.0f);
	planeVertex.uv = glm::vec2(5.0f, 0.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(10.0f, 0.0f, 10.0f);
	planeVertex.uv = glm::vec2(5.0f, 0.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(-10.0f, 0.0f, 10.0f);
	planeVertex.uv = glm::vec2(0.0f, 0.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(-10.0f, 0.0f, -10.0f);
	planeVertex.uv = glm::vec2(0.0f, 5.0f);
		vertices.push_back(planeVertex);

	CalculateTangents();
}

void Mesh::ImportMesh(vertex* vertArr, int vertexCount)
{

	this->nrOfVerticies = vertexCount;
	for (int i = 0; i < vertexCount; i++)
	{
		vertex vertexData = vertArr[i];
		vertexPolygon newVertex;
		newVertex.position = glm::vec3(vertexData.pos[0], vertexData.pos[1], vertexData.pos[2]);
		newVertex.uv = glm::vec2(vertexData.uv[0], vertexData.uv[1]);
		newVertex.normals = glm::vec3(vertexData.normal[0], vertexData.normal[1], vertexData.normal[2]);
		newVertex.tangent = glm::vec3(vertexData.tangent[0], vertexData.tangent[1], vertexData.tangent[2]);
		newVertex.bitangent = glm::vec3(vertexData.biNormal[0], vertexData.biNormal[1], vertexData.biNormal[2]);

		vertices.reserve(vertexCount);
		vertices.push_back(newVertex);
	}

}

void Mesh::CalculateTangents()
{
	// Normal and tangent Calculation
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 tangent = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 biTangent = glm::vec3(0.0f, 0.0f, 0.0f);
	float U1, V1, U2, V2 = 0.0f;

	float vertexVectorX, vertexVectorY, vertexVectorZ = 0.0f;
	glm::vec3 triangleEdge1 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 triangleEdge2 = glm::vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < vertices.size(); i += 3)
	{
		vertexVectorX = vertices[(i)].position.x - vertices[i + 2].position.x;
		vertexVectorY = vertices[(i)].position.y - vertices[i + 2].position.y;
		vertexVectorZ = vertices[(i)].position.z - vertices[i + 2].position.z;
		triangleEdge1 = glm::vec3(vertexVectorX, vertexVectorY, vertexVectorZ);


		vertexVectorX = vertices[i + 2].position.x - vertices[i + 1].position.x;
		vertexVectorY = vertices[i + 2].position.y - vertices[i + 1].position.y;
		vertexVectorZ = vertices[i + 2].position.z - vertices[i + 1].position.z;
		triangleEdge2 = glm::vec3(vertexVectorX, vertexVectorY, vertexVectorZ);

		// Gets the edges of the triangle and calculates the normal
		normal = glm::cross(triangleEdge1, triangleEdge2);
		normal = normalize(normal);

		// Get the UV coordinates to calculate a tangent aligned with the UV (using UV and vertex vectors)
		U1 = vertices[i].uv.x - vertices[i + 2].uv.x;
		V1 = vertices[i].uv.y - vertices[i + 2].uv.y;

		U2 = vertices[i + 2].uv.x - vertices[i + 1].uv.x;
		V2 = vertices[i + 2].uv.y - vertices[i + 1].uv.y;

		tangent.x = (U1 * triangleEdge1.x - V2 * triangleEdge2.x) * (1.0f / (U1 * V2 - U2 * V1));
		tangent.y = (U1 * triangleEdge1.y - V2 * triangleEdge2.y) * (1.0f / (U1 * V2 - U2 * V1));
		tangent.z = (U1 * triangleEdge1.z - V2 * triangleEdge2.z) * (1.0f / (U1 * V2 - U2 * V1));

		tangent = glm::normalize(tangent);

		vertices[i].tangent = tangent;
		vertices[i + 1].tangent = tangent;
		vertices[i + 2].tangent = tangent;

		biTangent = glm::cross(vertices[i].normals, tangent);
		vertices[i].bitangent = biTangent;
		vertices[i + 1].bitangent = biTangent;
		vertices[i + 2].bitangent = biTangent;

	}

}

void Mesh::setMaterial(unsigned int id)
{
	this->materialID = id;
}

unsigned int Mesh::getMaterialID() const
{
	return this->materialID;
}

glm::vec3 Mesh::GetPosition() const
{
	return this->worldPosition;
}

glm::vec3 Mesh::GetRotation() const
{
	return this->worldRotation;
}

void Mesh::setPosition(glm::vec3 newPos)
{
	worldPosition = newPos;
}

void Mesh::setPosition(float x, float y, float z)
{
	worldPosition = glm::vec3(x, y, z);
}

void Mesh::setPositionX(float x)
{
	worldPosition.x = x;
}

void Mesh::setPositionY(float y)
{
	worldPosition.y = y;
}

void Mesh::setPositionZ(float z)
{
	worldPosition.z = z;
}

void Mesh::SetRotation(float x, float y, float z)
{
	worldRotation = glm::vec3(x, y, z);
}

void Mesh::SetRotationX(float x)
{
	worldRotation.x = x;
}

void Mesh::SetRotationY(float y)
{
	worldRotation.y = y;
}

void Mesh::SetRotationZ(float z)
{
	worldRotation.z = z;
}

std::vector<vertexPolygon> Mesh::GetVertices()
{
	return vertices;
}

std::vector<vertexPolygon>& Mesh::ModifyVertices()
{
	return vertices;
}

int Mesh::getVertexCount() const
{
	return vertices.size();
}

