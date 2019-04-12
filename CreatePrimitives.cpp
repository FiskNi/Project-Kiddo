#include "CreatePrimitives.h"

CreatePrimitive::CreatePrimitive()
{
	this->worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->worldRotation = 0.0f;
}
CreatePrimitive::~CreatePrimitive()
{

}

void CreatePrimitive::CreateTriangleData()
{

	// 36 hardcoded vertices representing a cube
	//std::vector<vertexPolygon> vertices;
	vertexPolygon tests;
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, -0.5f);
	tests.uv = glm::vec2(1.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, -0.5f);
	tests.uv = glm::vec2(1.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices.push_back(tests);

	tests.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	tests.uv = glm::vec2(0.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	tests.uv = glm::vec2(0.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices.push_back(tests);

	tests.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	tests.uv = glm::vec2(1.0f, 1.0f);
	tests.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	tests.uv = glm::vec2(0.0f, 0.0f);
	tests.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);

	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, -0.5f);
	tests.uv = glm::vec2(1.0f, 1.0f);
	tests.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, 0.5f);
	tests.uv = glm::vec2(0.0f, 0.0f);
	tests.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices.push_back(tests);

	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(1.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, -0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	tests.uv = glm::vec2(0.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices.push_back(tests);

	tests.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, -0.5f);
	tests.uv = glm::vec2(1.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	tests.uv = glm::vec2(0.0f, 0.0f);
	tests.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(tests);
	tests.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices.push_back(tests);



}

void CreatePrimitive::CreatePlaneData()
{
	vertexPolygon tests;
	tests.position = glm::vec3(-3.0f, 0.0f, -3.0f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	tests.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(tests);

	tests.position = glm::vec3(3.0f, 0.0f, -3.0f);
	tests.uv = glm::vec2(1.0f, 1.0f);
	vertices.push_back(tests);

	tests.position = glm::vec3(3.0f, 0.0f, 03.0f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	vertices.push_back(tests);


	tests.position = glm::vec3(0.5f, 0.0f, 3.0f);
	tests.uv = glm::vec2(1.0f, 0.0f);
	vertices.push_back(tests);

	tests.position = glm::vec3(3.0f, 0.0f, 3.0f);
	tests.uv = glm::vec2(0.0f, 0.0f);
	vertices.push_back(tests);

	tests.position = glm::vec3(3.0f, 0.0f, 3.0f);
	tests.uv = glm::vec2(0.0f, 1.0f);
	vertices.push_back(tests);


}

void CreatePrimitive::setTextureID(GLuint texID)
{
	this->diffuseID = texID;
}

GLuint CreatePrimitive::getTextureID() const
{
	return this->diffuseID;
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
	worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
}

std::vector<vertexPolygon> CreatePrimitive::getvertexPolygons()
{
	return vertices;
}
