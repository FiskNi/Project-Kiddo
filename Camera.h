#pragma once

#include "Headers.h"
class Camera
{
private:
	void reCalcCamVecs();

public:
	glm::vec3 camPos;
	glm::vec3 face;
	glm::vec3 worldUp;
	glm::vec3 camUp;
	glm::vec3 camRight;

	glm::mat4 projMat;

	float camYaw;
	float camPitch;

	//Projection variables.
	float width;
	float height;
	float nearPlane;
	float farPlane;


	//Should only apply to some cameras.
	float mSpeed;
	//float zoom;
	float sensitivity;

	//The Camera Constructor
	Camera(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0), float camYaw = -90.0f, float camPitch = 0.0f, float width = 640.0f, float height = 480.0f, float nearPlane = 0.1f, float farPlane = 200.0f);

	//WASD camera movement controls. Need only be called in gameLoop if you want to move the specific camera.
	//Add custom key bindings for devs? Could send in letters as strings and use those as keybinds.
	void FPSCamControls(GLFWwindow * window, float deltaTime);

	//Calculates and returns the view matrix.
	glm::mat4 GetViewMatrix();

	glm::mat4 GetProjectionMatrix();

	void setWinSize(float width, float height);

	//Pitch and yaw dependent on mouse movement if active. If limitPitch true standard FPS cam, No Flipping.
	void pitchYawMouse(float xOffset, float yOffset, bool limitPitch);

	void setProjMat(float width, float height, float nearPlane, float farPlane);

};


