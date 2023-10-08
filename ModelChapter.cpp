#include "ModelChapter.h"
#include "Utility.h"
#include "Model.h"

int ModelChapter::Exe()
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

	Shader shader("Shader/coord_system.vs", "Shader/coord_system.fs");

	// create camera
	Camera camera;
	camera.SetMoveSpeed(10);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// record cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Utility::mouse_callback);
	glfwSetScrollCallback(window, Utility::scroll_callback);

	// some other parameter (render time/mix factor)
	float curRenderTime = 0;
	float lastRenderTime = 0;

	shader.use();
	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(Utility::FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	shader.setMat4("model", glm::mat4(1));
	shader.setMat4("proj", proj);

	Model model("Model/backpack/backpack.obj");

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

		shader.use();
		shader.setMat4("view", camera.GetLookAtMatrix());
		glm::vec3 viewPos = camera.GetCameraPos();
		shader.setVec3("viewPos", viewPos);

		model.Draw(shader);

		// swap framebuffer
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	shader.clear();

	// terminate
	glfwTerminate();
	std::cout << "Terminate From OpenGL" << std::endl;
	return 0;
}
