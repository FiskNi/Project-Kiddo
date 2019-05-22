#include "Camera.h"

void Camera::reCalcCamVecs()
{
	glm::vec3 face;
	face.x = cos(glm::radians(this->camYaw)) * cos(glm::radians(this->camPitch));
	face.y = sin(glm::radians(this->camPitch));
	face.z = sin(glm::radians(this->camYaw)) * cos(glm::radians(this->camPitch));

	this->face = glm::normalize(face);

	this->camRight = glm::normalize(glm::cross(this->face, this->worldUp));
	this->camUp = glm::normalize(glm::cross(this->camRight, this->face));	
}

Camera::Camera()
{
	this->camPos = glm::vec3(-25.5f, 29.38f, 27.06f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->camYaw = -45.f;
	this->camPitch = -36.5f;
	this->camRight = glm::vec3(1.0f, 0.0f, 0.0f);

	this->face = glm::vec3(0.0f, 0.0f, -10.0f);
	this->mSpeed = 3.5f;
	this->sensitivity = 0.15f;

	this->width = WIDTH;
	this->height = HEIGHT;

	this->nearPlane = 0.0001f;
	this->farPlane = 200.0f;


	setProjMat(this->width, this->height, this->nearPlane, this->farPlane);

	reCalcCamVecs();
}

void Camera::FPSCamControls(GLFWwindow * window, float deltaTime)
{
	float camSpeed = this->mSpeed*deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		this->camPos -= camSpeed * this->camRight;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		this->camPos += camSpeed * this->camRight;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		this->camPos += camSpeed * this->face;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		this->camPos -= camSpeed * this->face;
	if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
		this->camPos += camSpeed * this->camUp;
	if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
		this->camPos -= camSpeed * this->camUp;
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		this->camPitch += camSpeed * 10;
	if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
		this->camPitch -= camSpeed * 10;
	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		this->camYaw += camSpeed * 10;
	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		this->camYaw -= camSpeed * 10;

	reCalcCamVecs();
}

glm::mat4 Camera::GetViewMatrix()
{
	//Calculates the view matrix given the camera positions.
	return glm::lookAt(this->camPos, this->camPos + this->face, this->camUp);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return this->projMat;
}

void Camera::setWinSize(float width, float height)
{
	this->width = width;
	this->height = height;

	setProjMat(this->width, this->height, this->nearPlane, this->farPlane);
}

void Camera::setProjMat(float width, float height, float nearPlane, float farPlane)
{
	//this->projMat = glm::perspective((3.14f*0.45f), width / height, nearPlane, farPlane);
	this->projMat = glm::ortho(-8.f * 4.f, 8.f*4.f, -4.5f * 4.f, 4.5f*4.f, nearPlane, farPlane);
}