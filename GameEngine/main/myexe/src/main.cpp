#include <tuple>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm> 

#include "../include/Particle.hpp"
#include "../include/ShaderSources.hpp"

// function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::tuple<unsigned int, unsigned int, unsigned int> createVAO(double x, double y);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// global variables
static std::vector<Particle> particles;

int main()
{
	OpenGlWrapper openglWrapper(SCR_WIDTH, SCR_HEIGHT, "Game Engine Demo");
	opengl_wrapper::ShaderProgram_t shaderProgramm = openglWrapper.createProgram(vertexShaderSource, fragmentShaderSource);

	// game loops
	// -----------
	mathslib::Vector3 gravity = mathslib::Vector3(0, -20, 0);
	double frametime = 0.033333;//first frame considered at 30fps
	while (!glfwWindowShouldClose(window))
	{
		auto start(std::chrono::system_clock::now());
		// input
		// -----

		// logic
		auto particle = std::begin(particles);
		while (particle != std::end(particles)) 
		{
			particle->integrate(gravity, frametime);//use last frame time for integration
			if (!particle->isVisible(SCR_WIDTH, SCR_HEIGHT))
			{
				particle = particles.erase(particle);
			}
			else
			{
				++particle;
			}

		}

		// render
		// ------

		// cleaning screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// drawing particles
		glUseProgram(shaderProgram);
		for (auto& particle : particles) {
			std::tuple<unsigned int, unsigned int, unsigned int> bufferIDs = createVAO(particle.getPosition().getX(), particle.getPosition().getY());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glDeleteVertexArrays(1, &std::get<0>(bufferIDs));
			glDeleteBuffers(1, &std::get<1>(bufferIDs));
			glDeleteBuffers(1, &std::get<2>(bufferIDs));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		auto end(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsedSeconds = end - start;
		frametime = elapsedSeconds.count();//update frametime using last frame
	}


	return EXIT_SUCCESS;
}

// process all input: query GLFW whether relevant keys are pressed/released and react accordingly
// ---------------------------------------------------------------------------------------------------------
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// create buffers to contains graphical data
std::tuple<unsigned int, unsigned int, unsigned int> createVAO(double x, double y)
{
	double topX = x + 5;
	if (topX > SCR_WIDTH)
	{
		topX = SCR_WIDTH;
	}

	double bottomX = x - 5;
	if (bottomX < 0)
	{
		bottomX = 0;
	}


	double topY = y + 5;
	if (topY > SCR_HEIGHT)
	{
		topY = SCR_HEIGHT;
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



