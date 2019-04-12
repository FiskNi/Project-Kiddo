#include "CreatePrimitives.h"

CreatePrimitive::CreatePrimitive()
{
	this->gVertexBuffer = 0;
	this->gVertexAttribute = 0;
	this->worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->worldRotation = 0.0f;
	this->pos = glm::vec3(0, 0, -1);
}
CreatePrimitive::~CreatePrimitive()
{

}

void CreatePrimitive::CreateTriangleData(GLuint shaderID, float test)
{
	struct TriangleVertex 
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normals;
		glm::vec3 tangent;
		glm::vec3 biTangent;
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
	if (vertexPos == -1) 
		std::cout << "VertexPos is wrong" << std::endl;

	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(
		vertexPos,				// location in shader
		3,						// how many elements of type (see next argument)
		GL_FLOAT,				// type of each element
		GL_FALSE,				// integers will be normalized to [-1,1] or [0,1] when read...
		sizeof(TriangleVertex), // distance between two vertices in memory (stride)
		BUFFER_OFFSET(0)		// offset of FIRST vertex in the list.
	);

	GLint uvCoord = glGetAttribLocation(shaderID, "vertex_uv");
	if (uvCoord == -1)
		std::cout << "uvCoord is wrong" << std::endl;

	glVertexAttribPointer(
		uvCoord,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TriangleVertex),
		BUFFER_OFFSET(sizeof(float) * 3)
	);

	GLint normals = glGetAttribLocation(shaderID, "vertex_normal");
	if (uvCoord == -1)
		std::cout << "normal is wrong" << std::endl;

	glVertexAttribPointer(
		normals,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TriangleVertex),
		BUFFER_OFFSET(sizeof(float) * 5)
	);

	GLint tangent = glGetAttribLocation(shaderID, "vertex_tangent");
	if (tangent == -1)
		std::cout << "Tangent is wrong" << std::endl;

	glVertexAttribPointer(
		tangent,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TriangleVertex),
		BUFFER_OFFSET(sizeof(float) * 8)
	);

	GLint biTangent = glGetAttribLocation(shaderID, "vertex_bitangent");
	if (biTangent == -1)
		std::cout << "biTangent is wrong" << std::endl;

	glVertexAttribPointer(
		biTangent,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TriangleVertex),
		BUFFER_OFFSET(sizeof(float) * 11)
	);
}

void CreatePrimitive::setTextureID(GLuint texID)
{
	this->diffuseID = texID;
}

GLuint CreatePrimitive::getTextureID() const
{
	return this->diffuseID;
}

GLuint CreatePrimitive::getVertexAttribute() const
{
	return this->gVertexAttribute;
}

glm::vec3 CreatePrimitive::getWorldPosition() const
{
	return this->worldPosition;
}

float CreatePrimitive::getWorldRotation() const
{
	return this->worldRotation;
}

void CreatePrimitive::MovePrimitive(GLFWwindow *window, float deltaTime)
{
	float moveSpeed = 5.5f*deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		this->worldPosition = glm::vec3(this->worldPosition.x + moveSpeed,
			this->worldPosition.y, this->worldPosition.z);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		this->worldPosition = glm::vec3(this->worldPosition.x - moveSpeed,
			this->worldPosition.y, this->worldPosition.z);
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		this->worldPosition = glm::vec3(this->worldPosition.x, this->worldPosition.y,
			this->worldPosition.z + moveSpeed);

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		this->worldPosition = glm::vec3(this->worldPosition.x, this->worldPosition.y,
			this->worldPosition.z - moveSpeed);
}

void CreatePrimitive::setPosition()
{
	this->worldPosition = glm::vec3(2.0f, 4.0f, -1.0f);
}