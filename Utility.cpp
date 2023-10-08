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

unsigned int Utility::LoadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, numOfChannel;
	unsigned char* data = stbi_load(path, &width, &height, &numOfChannel, 0);
	if (data)
	{
		GLenum format;
		if (numOfChannel == 1)
		{
			format = GL_RED;
		}
		else if (numOfChannel == 3)
		{
			format = GL_RGB;
		}
		else if (numOfChannel == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture (" << path << ")" << std::endl;
	}

	// free texture data
	stbi_image_free(data);

	return textureID;
}
