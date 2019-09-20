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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void generateBuffers(const std::vector<Particle>& particles, std::vector<float>& vertices, std::vector<unsigned int>& indices,
	double widthMax, double heightMax);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// global variables
static std::vector<Particle> particles;

int main()
{
	opengl_wrapper::OpenGlWrapper openglWrapper(SCR_WIDTH, SCR_HEIGHT, "Game Engine Demo");
	GLFWwindow* mainWindow = openglWrapper.getMainWindow();

	openglWrapper.setKeyboardCallback(mainWindow, keyCallback);
	opengl_wrapper::ShaderProgram_t shadersProgramm = openglWrapper.createProgram(vertexShaderSource, fragmentShaderSource);
	openglWrapper.useShadersProgram(shadersProgramm);

	// game loops
	// -----------
	mathslib::Vector3 gravity = mathslib::Vector3(0, -20, 0);
	double frametime = 0.033333;//first frame considered at 30fps
	while (!openglWrapper.windowShouldClose(mainWindow))
	{
		auto start(std::chrono::system_clock::now());
		// input
		// -----
		openglWrapper.pollEvent();

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
		openglWrapper.clearCurrentWindow();

		// drawing particles
		std::vector<float> verticesBuffer;
		std::vector<unsigned int> indicesBuffer;
		generateBuffers(particles, verticesBuffer, indicesBuffer, SCR_WIDTH, SCR_HEIGHT);
		std::tuple<unsigned int, unsigned int, unsigned int> openglBuffers = openglWrapper.createAndBindVAO(verticesBuffer, indicesBuffer);
		openglWrapper.draw(GL_TRIANGLES, indicesBuffer.size());
		openglWrapper.cleanAndDeleteVAO(openglBuffers);

		openglWrapper.swapBuffers(mainWindow);

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

void generateBuffers(const std::vector<Particle>& particles, std::vector<float>& vertices, std::vector<unsigned int> & indices,
	double widthMax, double heightMax)
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





