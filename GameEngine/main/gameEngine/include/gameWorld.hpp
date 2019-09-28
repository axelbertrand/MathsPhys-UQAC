#pragma once

#include <string>

#include "openGlWrapper.hpp"
#include "particle.hpp"
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

	const opengl_wrapper::OpenGlWrapper m_openGlWrapper;
	GLFWwindow * const m_mainWindow;
	InputsManager m_inputsManager;
	std::vector<Particle> m_particles;

	std::vector<InputsManager::Intention> getPendingIntentions();

	void updateGame(const std::vector<InputsManager::Intention> pendingIntentions, const mathslib::Vector3& gravity, const double frametime);
	void processInputs(const std::vector<InputsManager::Intention>& pendingIntentions);
	void processIntention(InputsManager::Intention intention);
	void updateParticles(const mathslib::Vector3& gravity, const double frametime);

	void renderGame() const;
	std::tuple<std::vector<double>, std::vector<unsigned int>> generateBuffers() const;
};