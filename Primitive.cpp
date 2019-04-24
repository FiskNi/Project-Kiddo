#include "Primitive.h"

Primitive::Primitive()
{
	this->worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->worldRotation = 0.0f;

	gVertexBuffer = 0;
	gVertexAttribute = 0;
	
	vertex test;

}
Primitive::~Primitive()
{

}

//=============================================================
//	Creates a hardcoded cube primitive and binds it into a VAO buffer
//=============================================================
void Primitive::CreateCubeData()
{
	// 36 hardcoded vertices representing a cube

	vertexPolygon cubeVertex;
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	//cubeVertex.tangent = glm::vec3(0.0f);
	//cubeVertex.bitangent = glm::vec3(0.0f);

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

	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttribute);

	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttribute);

	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBuffer);

	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);

	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexPolygon), vertices.data(), GL_STATIC_DRAW);

	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(
		0,				// location in shader
		3,						// how many elements of type (see next argument)
		GL_FLOAT,				// type of each element
		GL_FALSE,				// integers will be normalized to [-1,1] or [0,1] when read...
		sizeof(vertexPolygon), // distance between two vertices in memory (stride)
		BUFFER_OFFSET(0)		// offset of FIRST vertex in the list.
	);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 3)
	);

	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 5)
	);

	glVertexAttribPointer(
		3,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 8)
	);

	glVertexAttribPointer(
		4,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 11)
	);
}

//=============================================================
//	Creates a hardcoded plane primitive and binds it into a VAO buffer
//=============================================================
void Primitive::CreatePlaneData()
{
	// 6 hardcoded vertices representing a plane
	vertexPolygon planeVertex;
	planeVertex.position = glm::vec3(-20.0f, 0.0f, -20.0f);
	planeVertex.uv = glm::vec2(0.0f, 5.0f);
	planeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	//planeVertex.tangent = glm::vec3(0.0f);
	//planeVertex.bitangent = glm::vec3(0.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(20.0f, 0.0f, -20.0f);
	planeVertex.uv = glm::vec2(5.0f, 5.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(20.0f, 0.0f, 20.0f);
	planeVertex.uv = glm::vec2(5.0f, 0.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(20.0f, 0.0f, 20.0f);
	planeVertex.uv = glm::vec2(5.0f, 0.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(-20.0f, 0.0f, 20.0f);
	planeVertex.uv = glm::vec2(0.0f, 0.0f);
		vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(-20.0f, 0.0f, -20.0f);
	planeVertex.uv = glm::vec2(0.0f, 5.0f);
		vertices.push_back(planeVertex);

	CalculateTangents();

	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttribute);

	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttribute);

	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBuffer);

	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);

	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexPolygon), vertices.data(), GL_STATIC_DRAW);

	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(
		0,				// location in shader
		3,						// how many elements of type (see next argument)
		GL_FLOAT,				// type of each element
		GL_FALSE,				// integers will be normalized to [-1,1] or [0,1] when read...
		sizeof(vertexPolygon), // distance between two vertices in memory (stride)
		BUFFER_OFFSET(0)		// offset of FIRST vertex in the list.
	);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 3)
	);

	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 5)
	);

	glVertexAttribPointer(
		3,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 8)
	);

	glVertexAttribPointer(
		4,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 11)
	);
}

void Primitive::ImportMesh(vertex*vertArr, int nrOfVerticies)
{

	vertexPolygon dummyVertex;

	for (int i = 0; i < nrOfVerticies; i++)
	{
		this->vertices.push_back(dummyVertex);
	}

	glGenVertexArrays(1, &this->gVertexAttribute);

	glBindVertexArray(this->gVertexAttribute);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBuffer);

	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, nrOfVerticies * sizeof(vertex), vertArr, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,				// location in shader
		3,						// how many elements of type (see next argument)
		GL_FLOAT,				// type of each element
		GL_FALSE,				// integers will be normalized to [-1,1] or [0,1] when read...
		sizeof(vertex), // distance between two vertices in memory (stride)
		BUFFER_OFFSET(0)		// offset of FIRST vertex in the list.
	);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertex),
		BUFFER_OFFSET(sizeof(float) * 3)
	);

	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertex),
		BUFFER_OFFSET(sizeof(float) * 5)
	);

}

void Primitive::CalculateTangents()
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

void Primitive::setMaterial(unsigned int id)
{
	this->materialID = id;
}

unsigned int Primitive::getMaterialID() const
{
	return this->materialID;
}

GLuint Primitive::getVertexAttribute() const
{
	return gVertexAttribute;
}

glm::vec3 Primitive::getPosition() const
{
	return this->worldPosition;
}

float Primitive::getRotation() const
{
	return this->worldRotation;
}

void Primitive::setPosition(glm::vec3 newPos)
{
	worldPosition = newPos;
}

std::vector<vertexPolygon> Primitive::getvertexPolygons()
{
	return vertices;
}

unsigned int Primitive::getPolygonCount() const
{
	return (unsigned)vertices.size();
}
