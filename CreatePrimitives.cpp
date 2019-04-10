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
	struct TriangleVertex {
		glm::vec3 position;
		glm::vec2 UVs;
		glm::vec3 normals;
	};

	// Create 1 triangle with 3 vertices
	std::vector<TriangleVertex> vertices;
	TriangleVertex tests;
	vertices.push_back(tests);
	vertices.push_back(tests);
	vertices.push_back(tests);
	vertices[0].position = glm::vec3((float)test, 0.5f, 0.3f);
	vertices[1].position = glm::vec3(0.5f, -0.5f, 0.3f);
	vertices[2].position = glm::vec3(-0.5f, -0.5f, 0.3f);


	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttribute);
	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttribute);
	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBuffer);
	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(TriangleVertex), vertices.data(), GL_STATIC_DRAW);

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

	/*repeat the process for the second attribute.
	query which "slot" corresponds to the input vertex_color in the Vertex Shader */
	GLuint vertexColor = glGetAttribLocation(shaderID, "uv_coords");
	glVertexAttribPointer(
		vertexColor,
		3,
		GL_FLOAT,
		GL_FALSE, sizeof(TriangleVertex), // distance between two vertexColor 
		BUFFER_OFFSET(sizeof(float) * 3)	// note, the first color starts after the first vertex.
	);
}

GLuint CreatePrimitive::getVertexAttribute() const
{
	return this->gVertexAttribute;
}