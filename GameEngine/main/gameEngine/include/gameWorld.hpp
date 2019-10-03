#pragma once

#include <string>
#include <vector>

#include "openGlWrapper.hpp"
#include "particle.hpp"
#include "particleForceGenerator.hpp"
#include "gravityForceGenerator.hpp"
#include "dragForceGenerator.hpp"
#include "forceRegister.hpp"
#include "../include/inputsManager.hpp"

class GameWorld
{
public:
	GameWorld();

	void run();
private:
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	const std::string WINDOW_TITLE = "Game Engine Demo";
	physicslib::ForceRegister forceRegister;
	physicslib::GravityForceGenerator gravityGenerator = physicslib::GravityForceGenerator(physicslib::Vector3(0, -20, 0));
	physicslib::DragForceGenerator dragGenerator = physicslib::DragForceGenerator(0.2, 0);

	const opengl_wrapper::OpenGlWrapper m_openGlWrapper;
	GLFWwindow * const m_mainWindow;
	InputsManager m_inputsManager;
	std::vector<std::shared_ptr<physicslib::Particle>> m_particles;

	std::vector<InputsManager::Intention> getPendingIntentions();

	void updateGame(const std::vector<InputsManager::Intention> pendingIntentions, const double frametime);
	void processInputs(const std::vector<InputsManager::Intention>& pendingIntentions);
	void processIntention(InputsManager::Intention intention);
	void updateParticles(const double frametime);

	void renderGame() const;
	std::tuple<std::vector<double>, std::vector<unsigned int>> generateBuffers() const;
};