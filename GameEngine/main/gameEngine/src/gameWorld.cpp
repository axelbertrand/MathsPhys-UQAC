#include "../include/gameWorld.hpp"

#include <chrono>
#include <algorithm>
#include <tuple>
#include <cmath>

#include "collisions/particleCable.hpp"

#include "../include/shaderSources.hpp"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

GameWorld::GameWorld(): m_openGlWrapper(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE), m_mainWindow(m_openGlWrapper.getMainWindow())
{
	// Initialize graphics
	m_openGlWrapper.setKeyboardCallback(m_mainWindow, keyCallback);
	m_shadersProgramm = m_openGlWrapper.createShadersProgram(vertexShaderSource, fragmentShaderSource);
	m_openGlWrapper.useShadersProgram(m_shadersProgramm);

	// Game variables
	glfwSetWindowUserPointer(m_mainWindow, &m_inputsManager); //save the manager's pointer to the window to be able to access it in the inputs callback function
}

void GameWorld::run()
{
	
	double frametime = 0.033333;//first frame considered at 30fps
	// game loops
	while (!m_openGlWrapper.windowShouldClose(m_mainWindow))
	{
		// manage frame time
		auto start(std::chrono::system_clock::now());

		// input
		auto pendingIntentions = getPendingIntentions();

		// logic
		updateGame(pendingIntentions, frametime);

		// render
		renderGame();

		// manage frame time
		auto end(std::chrono::system_clock::now());
		std::chrono::duration<double> elapsedSeconds = end - start;
		frametime = elapsedSeconds.count();//update frametime using last frame
	}
}

std::vector<InputsManager::Intention> GameWorld::getPendingIntentions()
{
	m_openGlWrapper.pollEvent(); //triggers the event callbacks
	std::vector<InputsManager::Intention> pendingIntentions = m_inputsManager.getPendingIntentions();
	m_inputsManager.clearIntentions();
	return pendingIntentions;
}

void GameWorld::updateGame(const std::vector<InputsManager::Intention> pendingIntentions, const double frametime)
{
	processInputs(pendingIntentions);
	updatePhysics(frametime);
}

void GameWorld::processInputs(const std::vector<InputsManager::Intention>& pendingIntentions)
{
	std::for_each(pendingIntentions.begin(), pendingIntentions.end(),
		[this](const InputsManager::Intention intention)
		{
			processIntention(intention);
		});
}

void GameWorld::processIntention(const InputsManager::Intention intention)
{
	if (intention == InputsManager::CLOSE_MAIN_WINDOW)
	{
		m_openGlWrapper.closeMainWindow();
	}
	else if (intention == InputsManager::CREATE_PARTICLE_ONE)
	{
		std::shared_ptr<physicslib::Particle> particle = std::make_shared<physicslib::Particle>(1, physicslib::Vector3(10, 10, 0), physicslib::Vector3(100, 50, 0), physicslib::Vector3());
		forceRegister.add(physicslib::ForceRegister::ForceRecord(particle, gravityGenerator));
		forceRegister.add(physicslib::ForceRegister::ForceRecord(particle, dragGenerator));
		m_particles.push_back(particle);
	}
	else if (intention == InputsManager::CREATE_PARTICLE_TWO)
	{
		std::shared_ptr<physicslib::Particle> particle = std::make_shared<physicslib::Particle>(1, physicslib::Vector3(10, 400, 0), physicslib::Vector3(100, 0, 0), physicslib::Vector3());
		forceRegister.add(physicslib::ForceRegister::ForceRecord(particle, gravityGenerator));
		forceRegister.add(physicslib::ForceRegister::ForceRecord(particle, dragGenerator));
		m_particles.push_back(particle);
	}
	else if (intention == InputsManager::CREATE_PARTICLE_THREE)
	{
		std::shared_ptr<physicslib::Particle> particle = std::make_shared<physicslib::Particle>(1, physicslib::Vector3(10, 10, 0), physicslib::Vector3(100, 100, 0), physicslib::Vector3());
		forceRegister.add(physicslib::ForceRegister::ForceRecord(particle, gravityGenerator));
		forceRegister.add(physicslib::ForceRegister::ForceRecord(particle, dragGenerator));
		m_particles.push_back(particle);
	}
}

void GameWorld::updatePhysics(const double frametime)
{
	forceRegister.updateAllForces(frametime);

	// update the position of the particles
	auto particle = std::begin(m_particles);
	while (particle != std::end(m_particles))
	{
		(*particle)->integrate(frametime); //use last frame time for integration
		if (!(*particle)->isVisible(SCR_WIDTH, SCR_HEIGHT))
		{
			forceRegister.remove(*particle);
			particle = m_particles.erase(particle);
		}
		else
		{
			++particle;
		}
	}

	//look for collisions
	auto particle1 = std::begin(m_particles);
	auto particle2 = std::begin(m_particles);
	while (particle1 != std::end(m_particles))
	{
		while (particle2 != particle1)
		{
			//physicslib::ParticleCable contactGenerator(**particle1, **particle2, 100, 1);
			//contactGenerator.addContact(contactRegister);

			if ((*particle1)->isInContactWith(**particle2) || (*particle1)->getDistance(**particle2) >= 100)
			{
				physicslib::ParticleContact particleContact(particle1->get(), particle2->get(), 1);
				contactRegister.add(particleContact);
			}

			particle2++;
		}
		/*if ((*particle1)->getPosition().getY() < 10)
		{
			physicslib::Particle particleGround = physicslib::Particle(0, physicslib::Vector3((*particle1)->getPosition().getX(), (*particle1)->getPosition().getY()-1, 0));
			physicslib::ParticleContact particleContact = physicslib::ParticleContact(particle1->get(), &particleGround, 0.5);
			contactRegister.add(particleContact);
		}*/
		particle1++;
	}
	contactRegister.resolveContacts(frametime);
	contactRegister.clear();
}

void GameWorld::renderGame() const
{
	// cleaning screen
	m_openGlWrapper.clearCurrentWindow();

	// drawing particles
	auto buffers = generateBuffers();
	std::vector<double> verticesBuffer = std::get<0>(buffers);
	std::vector<unsigned int> indicesBuffer = std::get<1>(buffers);
	auto openglBuffers = m_openGlWrapper.createAndBindDataBuffers(verticesBuffer, indicesBuffer);
	m_openGlWrapper.setUniformShaderVariable(m_shadersProgramm, "circleRadius", physicslib::Particle::PARTICLE_RADIUS);
	m_openGlWrapper.draw(GL_TRIANGLES, static_cast<unsigned int>(indicesBuffer.size()));
	m_openGlWrapper.cleanAndDeleteDataBuffers(openglBuffers);

	// swapping the double buffers
	m_openGlWrapper.swapGraphicalBuffers(m_mainWindow);
}

std::tuple<std::vector<double>, std::vector<unsigned int>> GameWorld::generateBuffers() const
{
	std::vector<double> vertices;
	std::vector<unsigned int> indices;

	unsigned int startIndex = 0;
	for (auto particle : m_particles)
	{
		double x = particle->getPosition().getX();
		double y = particle->getPosition().getY();

		double squareStep = physicslib::Particle::PARTICLE_RADIUS;
		double topX = std::min(x + squareStep, static_cast<double>(SCR_WIDTH));
		double bottomX = std::max(x - squareStep, 0.0);
		double topY = std::min(y + squareStep, static_cast<double>(SCR_HEIGHT));
		double bottomY = std::max(y - squareStep, 0.0);

		// top right
		vertices.push_back(topX);
		vertices.push_back(topY);
		vertices.push_back(0);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0);

		// bottom right
		vertices.push_back(topX);
		vertices.push_back(bottomY);
		vertices.push_back(0);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0);

		// bottom left
		vertices.push_back(bottomX);
		vertices.push_back(bottomY);
		vertices.push_back(0);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0);

		// top left
		vertices.push_back(bottomX);
		vertices.push_back(topY);
		vertices.push_back(0);
		vertices.push_back(x);
		vertices.push_back(y);
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
