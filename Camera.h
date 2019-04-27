#pragma once
#include "Headers.h"

class Camera
{
private:
	void reCalcCamVecs();

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

public:
	Camera();

	void FPSCamControls(GLFWwindow * window, float deltaTime);

	//Calculates and returns the view matrix.
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void setWinSize(float width, float height);

	//Pitch and yaw dependent on mouse movement if active. If limitPitch true standard FPS cam, No Flipping.
	void pitchYawMouse(float xOffset, float yOffset, bool limitPitch);
	void setProjMat(float width, float height, float nearPlane, float farPlane);

	glm::vec3 GetPosition() const { return camPos; }

};


