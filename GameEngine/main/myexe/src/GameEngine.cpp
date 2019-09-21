#include "..\include\GameEngine.hpp"
#include <vector3.hpp>
#include <iostream>
#include <chrono>
#include <algorithm>

using namespace mathslib;

std::vector<Particle> GameEngine::particles = std::vector<Particle>();

GameEngine::GameEngine(const std::string& title, unsigned int windowWidth, unsigned int windowHeight)
: mWindow(nullptr)
, WINDOW_WIDTH(windowWidth)
, WINDOW_HEIGHT(windowHeight)
{
	initGlfw();
	initWindow(title);
	initGlad();
	initShaders();

	particles.push_back(Particle(0.99, 1, Vector3(), Vector3(1, 2, 3), Vector3()));
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

void GameEngine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		particles.push_back(Particle(0.90, 50, mathslib::Vector3(10, 10, 0), mathslib::Vector3(100, 50, 0), mathslib::Vector3()));
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		particles.push_back(Particle(0.99, 10, mathslib::Vector3(10, 400, 0), mathslib::Vector3(100, 0, 0), mathslib::Vector3()));
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		particles.push_back(Particle(0.95, 1, mathslib::Vector3(10, 10, 0), mathslib::Vector3(100, 100, 0), mathslib::Vector3()));
	}
}

void GameEngine::run() {
	mathslib::Vector3 gravity = mathslib::Vector3(0, -20, 0);

	//first frame considered at 30fps
	double frametime = 0.033333;
	while (!glfwWindowShouldClose(mWindow))
	{
		auto start(std::chrono::system_clock::now());
		// input
		// -----

		// logic
		update(gravity, frametime);

		// render
		// ------
		render();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(mWindow);
		glfwPollEvents();

		auto end(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsedSeconds = end - start;
		frametime = elapsedSeconds.count();
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
	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title.c_str(), nullptr, nullptr);
	if (mWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		throw std::exception("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	glfwSetKeyCallback(mWindow, keyCallback);
}

void GameEngine::initGlad()
{
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

		throw std::exception("Failed to initialize GLAD");
	}
}

void GameEngine::initShaders()
{
	// build and compile our shader program
	// ------------------------------------

	const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   float x = 2.0*(aPos.x/800.0)-1;\n"
	"   float y = 2.0*(aPos.y/600.0)-1;\n"
	"   float z = aPos.z;\n"
	"   gl_Position = vec4(x, y, z, 1.0);\n"
	"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n\0";

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		throw std::exception("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		throw std::exception("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
	}

	// link shaders
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, vertexShader);
	glAttachShader(mShaderProgram, fragmentShader);
	glLinkProgram(mShaderProgram);

	// check for linking errors
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		throw std::exception("ERROR::SHADER::PROGRAM::LINKING_FAILED");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::tuple<unsigned int, unsigned int, unsigned int> createVAO(double x, double y)
{
	double topX = x + 5;
	if (topX > WINDOW_WIDTH)
	{
		topX = WINDOW_WIDTH;
	}

	double bottomX = x - 5;
	if (bottomX < 0)
	{
		bottomX = 0;
	}


	double topY = y + 5;
	if (topY > WINDOW_HEIGHT)
	{
		topY = WINDOW_HEIGHT;
	}

	double bottomY = y - 5;
	if (bottomY < 0)
	{
		bottomY = 0;
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// The float generates a warning in the compilation because every thing 
	// is in double until now. But for some reason, it doesn't work when we try
	// to force opengl to use double
	float vertices[] = {
		topX, topY, 0.0f, // top right
		topX, bottomY, 0.0f,  // bottom right
		bottomX, bottomY, 0.0f,  // bottom left
		bottomX, topY, 0.0f   // top left
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return { VAO, VBO, EBO };
}

void GameEngine::update(mathslib::Vector3 newAcceleration, double t) {
	auto particle = std::begin(particles);
	while (particle != std::end(particles))
	{
		particle->integrate(newAcceleration, t);
		if (!particle->isVisible(WINDOW_WIDTH, WINDOW_HEIGHT))
		{
			particle = particles.erase(particle);
		}
		else
		{
			++particle;
		}
	}
}

void GameEngine::render()
{
	// cleaning screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// drawing particles
	glUseProgram(mShaderProgram);
	for (auto& particle : particles) {
		std::tuple<unsigned int, unsigned int, unsigned int> bufferIDs = createVAO(particle.getPosition().getX(), particle.getPosition().getY());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDeleteVertexArrays(1, &std::get<0>(bufferIDs));
		glDeleteBuffers(1, &std::get<1>(bufferIDs));
		glDeleteBuffers(1, &std::get<2>(bufferIDs));
	}
}