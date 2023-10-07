#pragma once

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera
{
private:
	glm::vec3 mCameraPos;
	glm::vec3 mCameraFront;
	glm::vec3 mCameraUp;

	float mCameraMoveSpeed;

	float mCameraYawSpeed;
	float mYaw;

	float mMaxPitch;
	float mMinPitch;
	float mCameraPitchSpeed;
	float mPitch;

public:
	Camera();

	glm::mat4 GetLookAtMatrix();

	void ProcessCameraMoveInput(GLFWwindow* window, float timeDelta, double cursorX, double cursorY);

	void SetMoveSpeed(float speed);

	glm::vec3 GetCameraPos();
};