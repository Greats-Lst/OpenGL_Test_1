#include "MaterialChapter.h"
#include "Utility.h"
#include "stb_image.h"

int MaterialChapter::Exe()
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVerticesWithNormal), CubeVerticesWithNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(3);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Shader shader("Shader/lighting.vs", "Shader/lighting.fs");
	Shader shader2("Shader/direction_lighting.vs", "Shader/direction_lighting.fs");

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

	// create camera
	Camera camera;
	camera.SetMoveSpeed(10);

	// light VAO
	glm::vec3 lightPos = CubePositions[2];
	unsigned int lightingVAO;
	glGenVertexArrays(1, &lightingVAO);
	glBindVertexArray(lightingVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// record cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Utility::mouse_callback);
	glfwSetScrollCallback(window, Utility::scroll_callback);

	// some other parameter (render time/mix factor)
	float curRenderTime = 0;
	float lastRenderTime = 0;
	float mixFactor = 0.2f;

	shader.use();
	//shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	//shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	shader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
	shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	shader.setFloat("material.shinness", 32.0f);

	shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	// real render loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render delta time
		curRenderTime = (float)glfwGetTime();
		float deltaRenderTime = curRenderTime - lastRenderTime;
		lastRenderTime = curRenderTime;

		// input
		Utility::processInput(window);
		camera.ProcessCameraMoveInput(window, deltaRenderTime, Utility::CursorXOffset, Utility::CursorYOffset);
		Utility::ResetCursorOffset();

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

		shader.use();
		shader.setInt("outTexture0", 0);
		shader.setInt("outTexture1", 1);
		shader.setFloat("mixFactor", mixFactor);

		lightPos.x = 2.f * sin(curRenderTime * 2) + 1.f;
		lightPos.y = cos(curRenderTime / 2.f) + 2.f;
		shader.setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);

		// coord system
		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(Utility::FOV), 800.0f / 600.0f, 0.1f, 100.0f);
		shader.setMat4("proj", proj);
		shader.setMat4("view", camera.GetLookAtMatrix());
		glm::vec3 viewPos = camera.GetCameraPos();
		shader.setVec3("viewPos", viewPos.x, viewPos.y, viewPos.z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO[0]);
		glm::mat4 model(1.0f);
		for (size_t i = 1; i < 2; ++i)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, CubePositions[i]);
			//model = glm::rotate(model, curRenderTime * glm::radians(20.0f * i), glm::vec3(0.5f, 1.0f, 0.0f));
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// light
		shader2.use();
		shader2.setMat4("proj", proj);
		shader2.setMat4("view", camera.GetLookAtMatrix());
		shader2.setInt("outTexture0", 1);
		glBindVertexArray(lightingVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		shader2.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
