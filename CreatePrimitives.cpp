#include "CreatePrimitives.h"

CreatePrimitive::CreatePrimitive()
{
	this->gVertexBuffer = 0;
	this->gVertexAttribute = 0;
}
CreatePrimitive::~CreatePrimitive()
{

}

void CreatePrimitive::CreateTriangleData(GLuint shaderID, float test)
{
	struct TriangleVertex 
	{
		glm::vec3 position;
	};

	// 36 hardcoded vertices representing a cube
	std::vector<TriangleVertex> vertices;
	TriangleVertex tests;
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);

	tests.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	vertices.push_back(tests);

	tests.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);

	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);

	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	vertices.push_back(tests);

	tests.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, -0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	vertices.push_back(tests);

	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttribute);
	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttribute);
	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBuffer);
	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(TriangleVertex), vertices.data(), GL_STATIC_DRAW);

	// query which "slot" corresponds to the input vertex_position in the Vertex Shader 
	GLint vertexPos = glGetAttribLocation(shaderID, "vertex_position");
	// if this returns -1, it means there is a problem, and the program will likely crash.
	// examples: the name is different or missing in the shader

	if (vertexPos == -1) {
		std::cout << "VertexPos is wrong" << std::endl;
	}

	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(
		vertexPos,				// location in shader
		3,						// how many elements of type (see next argument)
		GL_FLOAT,				// type of each element
		GL_FALSE,				// integers will be normalized to [-1,1] or [0,1] when read...
		sizeof(TriangleVertex), // distance between two vertices in memory (stride)
		BUFFER_OFFSET(0)		// offset of FIRST vertex in the list.
	);
}

GLuint CreatePrimitive::getVertexAttribute() const
{
	return this->gVertexAttribute;
}