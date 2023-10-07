#include "TextureChapter.h"
#include "Utility.h"
#include "stb_image.h"

int TextureChapter::Exe()
{
	Utility::InitGLFW();

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

	if (!Utility::InitGLAD())
	{
		return -1;
	}

	// init viewport
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, Utility::framebuffer_size_callback);

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(RectangleVertices), RectangleVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Shader shader("Shader/texture.vs", "Shader/texture.fs");
	Shader shader2("Shader/vertex_position_color.vs", "Shader/vertex_position_color.fs");

	// create texture object
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load texture
	int width, height, numOfChannel;
	unsigned char* data = stbi_load("Texture/wall.jpg", &width, &height, &numOfChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture (Texture/wall.jpg)" << std::endl;
	}
	// free texture data
	stbi_image_free(data);

	// create texture object2;
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Texture/awesomeface.png", &width, &height, &numOfChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture (Texture/awesomeface.png)" << std::endl;
	}
	stbi_image_free(data);


	shader.use();
	shader.setInt("outTexture0", 0);
	shader.setInt("outTexture1", 1);
	float mixFactor = 0.2f;
	shader.setFloat("mixFactor", mixFactor);

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// real render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		Utility::processInput(window);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			mixFactor += 0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			mixFactor -= 0.01f;
		}
		mixFactor = fmax(mixFactor, 0.0f);
		mixFactor = fmin(mixFactor, 1.0f);
		shader.setFloat("mixFactor", mixFactor);

		// commands
		glClearColor(0.0f, 0.0f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = (float)glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.3f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "outColor");
		//for (int i = 0; i < 2; ++i)
		//{
		//	if (i == 0)
		//	{
		//		shader.use();
		//	}
		//	else
		//	{
		//		shader2.use();
		//	}

		//	//Note that finding the uniform location does not require you to use the shader program first, 
		//	//but updating a uniform DOES! require you to first use the program(by calling glUseProgram), 
		//	// because it sets the uniform on the currently active shader program.
		//	//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		//	glBindVertexArray(VAO[i]);
		//	glDrawArrays(GL_TRIANGLES, 0, 3);
		//}
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap framebuffer
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);
	shader.clear();
	shader2.clear();

	// terminate
	glfwTerminate();
	std::cout << "Terminate From OpenGL" << std::endl;
	return 0;
}