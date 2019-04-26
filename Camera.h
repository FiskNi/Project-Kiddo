#pragma once
#include "Headers.h"

class Camera
{
private:
	void reCalcCamVecs();

	bool locked = false;

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
	Camera();

	//WASD camera movement controls. Need only be called in gameLoop if you want to move the specific camera.
	//Add custom key bindings for devs? Could send in letters as strings and use those as keybinds.
	void FPSCamControls(GLFWwindow * window, float deltaTime);
	void followPlayer(glm::vec3 vec);
	//Calculates and returns the view matrix.
	glm::mat4 GetViewMatrix();

	glm::mat4 GetProjectionMatrix();

	void setWinSize(float width, float height);

	//Pitch and yaw dependent on mouse movement if active. If limitPitch true standard FPS cam, No Flipping.
	void pitchYawMouse(float xOffset, float yOffset, bool limitPitch);

	void setProjMat(float width, float height, float nearPlane, float farPlane);

};


