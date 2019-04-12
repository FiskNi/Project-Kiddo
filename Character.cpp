#include "Character.h"

Character::Character(GLuint shaderProgram)
{
	float temp = 0;
	characterModel.CreateTriangleData(shaderProgram, temp);
}


Character::~Character()
{

}

void Character::moveCharacter(GLFWwindow* window, float dTime)
{
	float movementChange = 1.0f;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		position = glm::vec3(position.x, position.y, position.z + movementChange);

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		position = glm::vec3(position.x, position.y, position.z - movementChange);

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		position = glm::vec3(position.x - movementChange, position.y, position.z);

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		position = glm::vec3(position.x + movementChange, position.y, position.z);

	//characterModel.setPostition(position);
}

CreatePrimitive Character::getMesh()
{
	return characterModel;
}
