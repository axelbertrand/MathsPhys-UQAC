#include "../include/gameEngine.hpp"

#include <chrono>
#include <algorithm>

#include "../include/shaderSources.hpp"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

GameEngine::GameEngine(): m_openGlWrapper(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE), m_mainWindow(m_openGlWrapper.getMainWindow())
{
	// Initialize graphics
	m_openGlWrapper.setKeyboardCallback(m_mainWindow, keyCallback);
	opengl_wrapper::ShaderProgram_t shadersProgramm = m_openGlWrapper.createShadersProgram(vertexShaderSource, fragmentShaderSource);
	m_openGlWrapper.useShadersProgram(shadersProgramm);

	// Game variables
	glfwSetWindowUserPointer(m_mainWindow, &m_inputsManager); //save the manager's pointer to the window to be able to access it in the inputs callback function
}

void GameEngine::run()
{
	const mathslib::Vector3 gravity = mathslib::Vector3(0, -20, 0);
	double frametime = 0.033333;//first frame considered at 30fps
	// game loops
	while (!m_openGlWrapper.windowShouldClose(m_mainWindow))
	{
		// manage frame time
		auto start(std::chrono::system_clock::now());

		// input
		auto pendingIntentions = getPendingIntentions();

		// logic
		updateGame(pendingIntentions, gravity, frametime);

		// render
		renderGame();

		// manage frame time
		auto end(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsedSeconds = end - start;
		frametime = elapsedSeconds.count();//update frametime using last frame
	}
}

std::vector<InputsManager::Intention> GameEngine::getPendingIntentions()
{
	m_openGlWrapper.pollEvent(); //triggers the event callbacks
	std::vector<InputsManager::Intention> pendingIntentions = m_inputsManager.getPendingIntentions();
	m_inputsManager.clearIntentions();
	return pendingIntentions;
}

void GameEngine::updateGame(const std::vector<InputsManager::Intention> pendingIntentions, const mathslib::Vector3& gravity, const double frametime)
{
	processInputs(pendingIntentions);
	updateParticles(gravity, frametime);
}

void GameEngine::processInputs(const std::vector<InputsManager::Intention>& pendingIntentions)
{
	std::for_each(pendingIntentions.begin(), pendingIntentions.end(),
		[this](const InputsManager::Intention intention)
		{
			processIntention(intention);
		});
}

void GameEngine::processIntention(const InputsManager::Intention intention)
{
	if (intention == InputsManager::CLOSE_MAIN_WINDOW)
	{
		m_openGlWrapper.closeMainWindow();
	}
	else if (intention == InputsManager::CREATE_PARTICLE_ONE)
	{
		m_particles.push_back(Particle(0.90, 50, mathslib::Vector3(10, 10, 0), mathslib::Vector3(100, 50, 0), mathslib::Vector3()));
	}
	else if (intention == InputsManager::CREATE_PARTICLE_TWO)
	{
		m_particles.push_back(Particle(0.99, 10, mathslib::Vector3(10, 400, 0), mathslib::Vector3(100, 0, 0), mathslib::Vector3()));
	}
	else if (intention == InputsManager::CREATE_PARTICLE_THREE)
	{
		m_particles.push_back(Particle(0.95, 1, mathslib::Vector3(10, 10, 0), mathslib::Vector3(100, 100, 0), mathslib::Vector3()));
	}
}

void GameEngine::updateParticles(const mathslib::Vector3& gravity, const double frametime)
{
	// update the position of the particles
	auto particle = std::begin(m_particles);
	while (particle != std::end(m_particles))
	{
		particle->integrate(gravity, frametime); //use last frame time for integration
		if (!particle->isVisible(SCR_WIDTH, SCR_HEIGHT))
		{
			particle = m_particles.erase(particle);
		}
		else
		{
			++particle;
		}
	}
}

void GameEngine::renderGame() const
{
	// cleaning screen
	m_openGlWrapper.clearCurrentWindow();

	// drawing particles
	auto buffers = generateBuffers();
	std::vector<double> verticesBuffer = std::get<0>(buffers);
	std::vector<unsigned int> indicesBuffer = std::get<1>(buffers);
	auto openglBuffers = m_openGlWrapper.createAndBindDataBuffers(verticesBuffer, indicesBuffer);
	m_openGlWrapper.draw(GL_TRIANGLES, static_cast<unsigned int>(indicesBuffer.size()));
	m_openGlWrapper.cleanAndDeleteDataBuffers(openglBuffers);

	// swapping the double buffers
	m_openGlWrapper.swapGraphicalBuffers(m_mainWindow);
}

std::tuple<std::vector<double>, std::vector<unsigned int>> GameEngine::generateBuffers() const
{
	std::vector<double> vertices;
	std::vector<unsigned int> indices;

	unsigned int startIndex = 0;
	for (const Particle& particle : m_particles)
	{
		double x = particle.getPosition().getX();
		double y = particle.getPosition().getY();

		double topX = std::min(x + 5, static_cast<double>(SCR_WIDTH));
		double bottomX = std::max(x - 5, 0.0);
		double topY = std::min(y + 5, static_cast<double>(SCR_HEIGHT));
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

	return { vertices, indices };
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputsManager* const inputsManager = static_cast<InputsManager * const>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		inputsManager->addIntention(InputsManager::Intention::CLOSE_MAIN_WINDOW);
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		inputsManager->addIntention(InputsManager::Intention::CREATE_PARTICLE_ONE);
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		inputsManager->addIntention(InputsManager::Intention::CREATE_PARTICLE_TWO);
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		inputsManager->addIntention(InputsManager::Intention::CREATE_PARTICLE_THREE);
	}

}
