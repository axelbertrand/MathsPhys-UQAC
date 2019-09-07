#include "..\include\GameEngine.hpp"
#include <iostream>
#include <chrono>
#include <algorithm>

GameEngine::GameEngine(const std::string& title, unsigned int windowWidth, unsigned int windowHeight)
: WINDOW_WIDTH(windowWidth)
, WINDOW_HEIGHT(windowHeight)
{
	initGlfw();
	initWindow(title);
	initGlad();
}

GameEngine::~GameEngine() {
	glfwTerminate();
}

void GameEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void GameEngine::run() {
    double t = 0.033;

	// game loop
	// -----------
	while (!glfwWindowShouldClose(mWindow))
	{
		// input
		// -----
		processInput();

		// update
		// ------
		auto start(std::chrono::system_clock::now());
		update(t);
		auto end(std::chrono::system_clock::now());

		std::chrono::duration<double> elapsedSeconds = end - start;
		t = elapsedSeconds.count();

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

void GameEngine::initGlfw()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

void GameEngine::initWindow(const std::string& title)
{
	// glfw window creation
	// --------------------
	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title.c_str(), NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		throw std::exception("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, GameEngine::framebuffer_size_callback);
}

void GameEngine::initGlad()
{
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

		throw std::exception("Failed to initialize GLAD");
	}
}

void GameEngine::processInput()
{
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(mWindow, true);
	}
}

void GameEngine::update(double t) {
	std::cout << "t = " << t << std::endl;
}