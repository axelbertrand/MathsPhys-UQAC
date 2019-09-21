#ifdef _WIN32
#define NOMINMAX
#endif

#include <tuple>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm> 

#include "../include/Particle.hpp"
#include "../include/ShaderSources.hpp"
#include "OpenGlWrapper.hpp"

// function definitions
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLFWwindow* initGraphics(const opengl_wrapper::OpenGlWrapper& openglWrapper, const unsigned int width, 
	const unsigned int height, const char* vertexShaderSource, const char* fragmentShaderSource);
void updateGame(std::vector<Particle> particles, const mathslib::Vector3& gravity, const double frametime);
void renderGame(const opengl_wrapper::OpenGlWrapper& openglWrapper, GLFWwindow* const mainWindow, const std::vector<Particle>& particles);
void generateBuffers(const std::vector<Particle>& particles, std::vector<float>& vertices, std::vector<unsigned int>& indices,
	const double widthMax, const double heightMax);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// Initialize graphics
	opengl_wrapper::OpenGlWrapper openglWrapper(SCR_WIDTH, SCR_HEIGHT, "Game Engine Demo");
	GLFWwindow* mainWindow = initGraphics(openglWrapper, SCR_WIDTH, SCR_HEIGHT, vertexShaderSource, fragmentShaderSource);

	// Game variables
	std::vector<Particle> particles;
	glfwSetWindowUserPointer(mainWindow, &particles); //save the particles' pointer to the window to be able to access it in the inputs callback function
	const mathslib::Vector3 gravity = mathslib::Vector3(0, -20, 0);
	double frametime = 0.033333;//first frame considered at 30fps

	// game loops
	while (!openglWrapper.windowShouldClose(mainWindow))
	{
		// manage frame time
		auto start(std::chrono::system_clock::now());

		// input
		openglWrapper.pollEvent();

		// logic
		updateGame(particles, gravity, frametime);

		// render
		renderGame(openglWrapper, mainWindow, particles);

		// manage frame time
		auto end(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsedSeconds = end - start;
		frametime = elapsedSeconds.count();//update frametime using last frame
	}

	return EXIT_SUCCESS;
}

GLFWwindow* initGraphics(const opengl_wrapper::OpenGlWrapper& openglWrapper, const unsigned int width, 
	const unsigned int height, const char * vertexShaderSource, const char * fragmentShaderSource)
{
	GLFWwindow* mainWindow = openglWrapper.getMainWindow();
	openglWrapper.setKeyboardCallback(mainWindow, keyCallback);
	opengl_wrapper::ShaderProgram_t shadersProgramm = openglWrapper.createShadersProgram(vertexShaderSource, fragmentShaderSource);
	openglWrapper.useShadersProgram(shadersProgramm);

	return mainWindow;
}

void updateGame(std::vector<Particle> particles, const mathslib::Vector3& gravity, const double frametime)
{
	// update the position of the particles
	auto particle = std::begin(particles);
	while (particle != std::end(particles))
	{
		particle->integrate(gravity, frametime); //use last frame time for integration
		if (!particle->isVisible(SCR_WIDTH, SCR_HEIGHT))
		{
			particle = particles.erase(particle);
		}
		else
		{
			++particle;
		}
	}
}

void renderGame(const opengl_wrapper::OpenGlWrapper& openglWrapper, GLFWwindow * const mainWindow, const std::vector<Particle>& particles)
{
	// cleaning screen
	openglWrapper.clearCurrentWindow();

	// drawing particles
	std::vector<float> verticesBuffer;
	std::vector<unsigned int> indicesBuffer;
	generateBuffers(particles, verticesBuffer, indicesBuffer, SCR_WIDTH, SCR_HEIGHT);
	std::tuple<unsigned int, unsigned int, unsigned int> openglBuffers = openglWrapper.createAndBindDataBuffers(verticesBuffer, indicesBuffer);
	openglWrapper.draw(GL_TRIANGLES, indicesBuffer.size());
	openglWrapper.cleanAndDeleteDataBuffers(openglBuffers);

	// swapping the double buffers
	openglWrapper.swapGraphicalBuffers(mainWindow);
}

// process all input: query GLFW whether relevant keys are pressed/released and react accordingly
// ---------------------------------------------------------------------------------------------------------
void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	std::vector<Particle>* particles = (std::vector<Particle> * )glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		particles->push_back(Particle(0.90, 50, mathslib::Vector3(10, 10, 0), mathslib::Vector3(100, 50, 0), mathslib::Vector3()));
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		particles->push_back(Particle(0.99, 10, mathslib::Vector3(10, 400, 0), mathslib::Vector3(100, 0, 0), mathslib::Vector3()));
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		particles->push_back(Particle(0.95, 1, mathslib::Vector3(10, 10, 0), mathslib::Vector3(100, 100, 0), mathslib::Vector3()));
	}

}

void generateBuffers(const std::vector<Particle>& particles, std::vector<float>& vertices, std::vector<unsigned int> & indices,
	const double widthMax, const double heightMax)
{
	unsigned int startIndex = 0;
	for (const Particle & particle : particles) 
	{
		double x = particle.getPosition().getX();
		double y = particle.getPosition().getY();

		double topX = std::min(x + 5, widthMax);
		double bottomX = std::max(x - 5, 0.0);
		double topY = std::min(y + 5, heightMax);
		double bottomY = std::max(y - 5, 0.0);

		// top right
		vertices.push_back(topX);
		vertices.push_back(topY);
		vertices.push_back(0);

		// bottom right
		vertices.push_back(topX);
		vertices.push_back(bottomY);
		vertices.push_back(0);

		// bottom left
		vertices.push_back(bottomX);
		vertices.push_back(bottomY);
		vertices.push_back(0);

		// top left
		vertices.push_back(bottomX);
		vertices.push_back(topY);
		vertices.push_back(0);

		// first triangle
		indices.push_back(startIndex);
		indices.push_back(startIndex + 1);
		indices.push_back(startIndex + 3);

		// second triangle
		indices.push_back(startIndex + 1);
		indices.push_back(startIndex + 2);
		indices.push_back(startIndex + 3);

		startIndex += 4;
	}
}
