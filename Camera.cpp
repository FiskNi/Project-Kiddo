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

Camera::Camera() : face(glm::vec3(0, 0, -1)), mSpeed(3.5f), sensitivity(0.15f)//, zoom(40.0f)
{
	this->camPos = glm::vec3(0, 4, -10);
	this->worldUp = glm::vec3(0, 1, 0);
	this->camYaw = 90.0f;
	this->camPitch = 0.0f;

	this->width = WIDTH;
	this->height = HEIGHT;

	this->nearPlane = 0.1f;
	this->farPlane = 200.0f;


	setProjMat(this->width, this->height, this->nearPlane, this->farPlane);

	reCalcCamVecs();
}

void Camera::FPSCamControls(GLFWwindow * window, float deltaTime)
{
	float camSpeed = this->mSpeed*deltaTime;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->camPos -= camSpeed * this->camRight;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		this->camPos += camSpeed * this->camRight;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		this->camPos += camSpeed * this->face;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		this->camPos -= camSpeed * this->face;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		this->camPos += camSpeed * this->camUp;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		this->camPos -= camSpeed * this->camUp;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		this->camPos += camSpeed * this->camYaw;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		this->camPos -= camSpeed * this->camYaw;
	// Add camera pitch/yaw here
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

void Camera::pitchYawMouse(float xOffset, float yOffset, bool limitPitch = true)
{
	xOffset *= this->sensitivity;
	yOffset *= this->sensitivity;

	this->camYaw += xOffset;
	this->camPitch += yOffset;

	if (limitPitch)
	{
		if (this->camPitch > 89.0f)
			this->camPitch = 89.0f;
		if (this->camPitch < -89.0f)
			this->camPitch = 89.0f;
	}

	reCalcCamVecs();
}

void Camera::setProjMat(float width, float height, float nearPlane, float farPlane)
{
	this->projMat = glm::perspective((3.14f*0.45f), width / height, nearPlane, farPlane);
}