#include "Camera.h"

Camera::Camera()
{
	mCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mCameraMoveSpeed = 1.0f;

	mCameraYawSpeed = 10.0f;
	mYaw = -90;

	mMinPitch = -90;
	mMaxPitch = 90;
	mCameraPitchSpeed = 10.0f;
	mPitch = 0;
}

glm::mat4 Camera::GetLookAtMatrix()
{
	// glm 

	return glm::lookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp);

	// use camera coordinate

	//glm::vec3 cDir = -mCameraFront;
	//glm::vec3 cRight = glm::normalize(glm::cross(mCameraUp, cDir));
	//glm::vec3 cUp = glm::normalize(glm::cross(cDir, cRight));
	//glm::mat4 affine = glm::mat4(
	//	cRight.x, cRight.y, cRight.z, 0.0f,
	//	cUp.x, cUp.y, cUp.z, 0.0f,
	//	cDir.x, cDir.y, cDir.z, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f
	//);
	//affine = glm::transpose(affine);
	//glm::mat4 trans = glm::mat4(
	//	1.0f, 0.0f, 0.0f, -mCameraPos.x,
	//	0.0f, 1.0f, 0.0f, -mCameraPos.y,
	//	0.0f, 0.0f, 1.0f, -mCameraPos.z,
	//	0.0f, 0.0f, 0.0f, 1.0f
	//);
	//trans = glm::transpose(trans);
	//return affine * trans;
}

void Camera::ProcessCameraMoveInput(GLFWwindow* window, float timeDelta, double cursorXOffset, double cursorYOffset)
{
	// forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mCameraPos += mCameraFront * mCameraMoveSpeed * timeDelta;
	}

	// backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mCameraPos -= mCameraFront * mCameraMoveSpeed * timeDelta;
	}

	// right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		auto rightDir = glm::normalize(glm::cross(mCameraFront, mCameraUp));
		mCameraPos += rightDir * mCameraMoveSpeed * timeDelta;
	}

	// left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		auto rightDir = glm::normalize(glm::cross(mCameraFront, mCameraUp));
		mCameraPos -= rightDir * mCameraMoveSpeed * timeDelta;
	}

	// yaw
	mYaw += mCameraYawSpeed * cursorXOffset * timeDelta;

	// pitch
	mPitch -= mCameraPitchSpeed * cursorYOffset * timeDelta;
	if (mPitch < mMinPitch)
	{
		mPitch = mMinPitch;
	}
	if (mPitch > mMaxPitch)
	{
		mPitch = mMaxPitch;
	}

	mCameraFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mCameraFront.y = sin(glm::radians(mPitch));
	mCameraFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
}

void Camera::SetMoveSpeed(float speed)
{
	mCameraMoveSpeed = speed;
}

glm::vec3 Camera::GetCameraPos()
{
	return mCameraPos;
}

glm::vec3 Camera::GetCameraForward()
{
	return mCameraFront;
}

