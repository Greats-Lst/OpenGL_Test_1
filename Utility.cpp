#include "Utility.h"

double Utility::CursorXOffset = 0;
double Utility::CursorYOffset = 0;
double Utility::CurCursorX = -1;
double Utility::CurCursorY = -1;
float Utility::FOV = 45.0f;

void Utility::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Utility::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (CurCursorX == -1)
	{
		CurCursorX = xpos;
	}
	CursorXOffset = xpos - CurCursorX;
	CurCursorX = xpos;

	if (CurCursorY == -1)
	{
		CurCursorY = ypos;
	}
	CursorYOffset = ypos - CurCursorY;
	CurCursorY = ypos;
}

void Utility::ResetCursorOffset()
{
	CursorXOffset = 0;
	CursorYOffset = 0;
}

void Utility::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	FOV -= (float)yOffset;
	if (FOV < 1.0f)
	{
		FOV = 1.0f;
	}
	if (FOV > 45.0f)
	{
		FOV = 45.f;
	}
}

void Utility::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void Utility::InitGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool Utility::InitGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return false;
	}

	return true;
}