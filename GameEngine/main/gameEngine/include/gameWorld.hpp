#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "openGlWrapper.hpp"
#include "particle.hpp"
#include "forceGenerator/particleForceGenerator.hpp"
#include "forceGenerator/gravityForceGenerator.hpp"
#include "forceGenerator/dragForceGenerator.hpp"
#include "forceGenerator/forceRegister.hpp"
#include "collisions/contactRegister.hpp"
#include "collisions/particleContact.hpp"
#include "../include/inputsManager.hpp"
#include "../include/blob.hpp"

class GameWorld
{
public:
	GameWorld();

	void run();
private:
	enum ShaderProgrammType
	{
		PARTICLE,
		BACKGROUND
	};

	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	const unsigned int FLOOR_LEVEL = 100;
	const unsigned int CEILING_LEVEL = 590;
	const unsigned int LEFT_WALL_LIMIT = 10;
	const unsigned int RIGHT_WALL_LIMIT = 790;
	const unsigned int WATER_LIMIT = 600;
	const std::string WINDOW_TITLE = "Game Engine Demo";
	std::unordered_map<ShaderProgrammType, opengl_wrapper::ShaderProgram_t> m_shaderProgramms;
	physicslib::ForceRegister m_forceRegister;
	physicslib::ContactRegister m_contactRegister;
	std::shared_ptr<physicslib::GravityForceGenerator> gravityGenerator = std::make_shared<physicslib::GravityForceGenerator>(physicslib::Vector3(0, -20, 0));
	std::shared_ptr<physicslib::DragForceGenerator> dragGenerator = std::make_shared<physicslib::DragForceGenerator>(0.03, 0);

	const opengl_wrapper::OpenGlWrapper m_openGlWrapper;
	GLFWwindow* const m_mainWindow;
	InputsManager m_inputsManager;
	std::vector<std::shared_ptr<physicslib::Particle>> m_particles;
	std::vector<std::shared_ptr<Blob>> m_blobs;
	std::shared_ptr<Blob> m_mainBlob = nullptr;

	void updateGame(const std::vector<InputsManager::Intention> pendingIntentions, const double frametime);
	std::vector<InputsManager::Intention> getPendingIntentions();
	void processInputs(const std::vector<InputsManager::Intention>& pendingIntentions);
	void processIntention(InputsManager::Intention intention);
	std::shared_ptr<Blob> createBlob(const unsigned int blobCount);
	void separateMainBlob();
	void updatePhysics(const double frametime);
	void generateAllForces();
	void generateGravityAndDragForces();
	void generateBuoyancyForces();
	void generateBlobsForces();
	void detectCollision();
	void updateParticlesPosition(const double frametime);

	void renderGame() const;
	void drawBackground() const;
	void drawParticles() const;
	std::tuple<std::vector<double>, std::vector<unsigned int>> generateParticlesBuffers() const;
	std::tuple<std::vector<double>, std::vector<unsigned int>> generateBackgroundBuffers() const;
};