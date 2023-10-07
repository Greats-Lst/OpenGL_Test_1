#include "HelloTriangleAssignment.h"
#include "Utility.h"

int HelloTriangleAssignment::Exe()
{
	// init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "HelloWindow", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// set context
	glfwMakeContextCurrent(window);

	// init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	// init viewport
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, Utility::framebuffer_size_callback);

	// init vertices
	float vertices[] = {
		-0.5f, 0.5f, 0.0f,
		-0.1f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	float vertices2[] = {
		0.5f, 0.5f, 0.0f,
		0.1f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	// init VBO/VAO
	unsigned int VAO[2];
	unsigned int VBO[2];
	unsigned int EBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);
	for (int i = 0; i < 2; ++i)
	{
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), i == 0 ? vertices : vertices2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
	}


	int numberOfVertexAttribute;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numberOfVertexAttribute);
	std::cout << "Maximun number of vertex attibute supported : " << numberOfVertexAttribute << std::endl;

	// vertex shader
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];

	// test vertex shader compile success
	{
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (success == false)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cout << "Error - Shader - Vertex - Compilation Fail\n" << infoLog << std::endl;
		}
	}

	// fragment shader
	const char* fragShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSource, nullptr);
	glCompileShader(fragShader);

	// test fragment shader compile success
	{
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (success == false)
		{
			glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
			std::cout << "Error - Shader - Fragment - Compilation Fail\n" << infoLog << std::endl;
		}
	}

	const char* fragShaderSource2 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	unsigned int fragShader2;
	fragShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader2, 1, &fragShaderSource2, nullptr);
	glCompileShader(fragShader2);

	// create shader program to link vertex shader and fragment shader
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragShader2);
	glLinkProgram(shaderProgram2);

	// test shader program link success
	{
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
			std::cout << "Error - Shader - Link Fail\n" << infoLog << std::endl;
		}
	}

	// no longer need vertexShader and fragmentShader
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
	glDeleteShader(fragShader2);

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// real render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		Utility::processInput(window);

		// commands
		glClearColor(0.0f, 0.0f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < 2; ++i)
		{
			glUseProgram(i == 0 ? shaderProgram : shaderProgram2);
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		// swap framebuffer
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);

	// terminate
	glfwTerminate();
	std::cout << "Terminate From OpenGL" << std::endl;
	return 0;
}