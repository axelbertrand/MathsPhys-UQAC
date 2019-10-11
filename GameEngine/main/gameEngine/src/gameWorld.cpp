#include "../include/gameWorld.hpp"

#include <chrono>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <iostream>

#include "collisions/particleCable.hpp"

#include "forceGenerator/particleBuoyancyForceGenerator.hpp"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

GameWorld::GameWorld(): m_openGlWrapper(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE), m_mainWindow(m_openGlWrapper.getMainWindow())
{
	// Initialize graphics
	m_openGlWrapper.setKeyboardCallback(m_mainWindow, keyCallback);

	// Register particle shader
	opengl_wrapper::Shader particleShader;
	particleShader.loadFromFile("resources/shaders/particle.vs", "resources/shaders/particle.fs");
	m_shaderPrograms.insert(std::make_pair(ShaderProgramType::PARTICLE, particleShader));

	// Register background shader
	opengl_wrapper::Shader backgroundShader;
	backgroundShader.loadFromFile("resources/shaders/background.vs", "resources/shaders/background.fs");
	m_shaderPrograms.insert(std::make_pair(ShaderProgramType::BACKGROUND, backgroundShader));

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
	std::vector<InputsManager::Intention> pendingIntentions = m_inputsManager.getPendingIntentions(m_mainWindow);
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
	else if (intention == InputsManager::CREATE_BLOB)
	{
		std::shared_ptr<Blob> blob = createBlob(20);
		if (m_mainBlob == nullptr)
		{
			m_mainBlob = blob;
		}
	}
	else if (intention == InputsManager::CREATE_PARTICLE_TWO)
	{
		std::shared_ptr<physicslib::Particle> particle = std::make_shared<physicslib::Particle>
			(1, physicslib::Vector3(30, 30, 0), physicslib::Vector3(100, 50, 0), physicslib::Vector3());
		m_particles.push_back(particle);
	}
	else if (intention == InputsManager::CREATE_PARTICLE_THREE)
	{
		std::shared_ptr<physicslib::Particle> particle = std::make_shared<physicslib::Particle>
			(1, physicslib::Vector3(300, 30, 0), physicslib::Vector3(-100, 50, 0), physicslib::Vector3());
		m_particles.push_back(particle);
	}
	else if (intention == InputsManager::MOVE_BLOB_BOTTOM && m_mainBlob != nullptr)
	{
		std::shared_ptr<physicslib::GravityForceGenerator> moveBottomGenerator = std::make_shared<physicslib::GravityForceGenerator>
			(physicslib::Vector3(0, -300, 0));
		physicslib::ForceRegister::ForceRecord record(m_mainBlob->getCore(), moveBottomGenerator);
		m_forceRegister.add(record);
	}
	else if (intention == InputsManager::MOVE_BLOB_TOP && m_mainBlob != nullptr)
	{
		std::shared_ptr<physicslib::GravityForceGenerator> moveTopGenerator = std::make_shared<physicslib::GravityForceGenerator>
			(physicslib::Vector3(0, 1000, 0));
		physicslib::ForceRegister::ForceRecord record(m_mainBlob->getCore(), moveTopGenerator);
		m_forceRegister.add(record);
	}
	else if (intention == InputsManager::MOVE_BLOB_LEFT && m_mainBlob != nullptr)
	{
		std::shared_ptr<physicslib::GravityForceGenerator> moveLeftGenerator = std::make_shared<physicslib::GravityForceGenerator>
			(physicslib::Vector3(-500, 0, 0));
		physicslib::ForceRegister::ForceRecord record(m_mainBlob->getCore(), moveLeftGenerator);
		m_forceRegister.add(record);
	}
	else if (intention == InputsManager::MOVE_BLOB_RIGHT && m_mainBlob != nullptr)
	{
		std::shared_ptr<physicslib::GravityForceGenerator> moveLeftGenerator = std::make_shared<physicslib::GravityForceGenerator>
			(physicslib::Vector3(500, 0, 0));
		physicslib::ForceRegister::ForceRecord record(m_mainBlob->getCore(), moveLeftGenerator);
		m_forceRegister.add(record);
	}
	else if (intention == InputsManager::CUT_MAIN_BLOB && m_mainBlob != nullptr)
	{
		separateMainBlob();
	}
	else if (intention == InputsManager::FUSION_ALL_BLOB && m_blobs.size() > 1)
	{
		std::shared_ptr<physicslib::Particle > coreParticle = m_mainBlob->getCore();
		std::vector<std::shared_ptr<physicslib::Particle>> particles = m_mainBlob->getParticles();
		double linkElasticity = m_mainBlob->getLinksElasticity();
		double linkLength = m_mainBlob->getLinksRestingLength();

		std::for_each(m_blobs.begin(), m_blobs.end(),
			[this, &particles](const std::shared_ptr<Blob> blob)
			{
				if (blob != m_mainBlob)
				{
					particles.push_back(blob->getCore());
					std::vector<std::shared_ptr<physicslib::Particle>> newParticles = blob->getParticles();
					particles.insert(particles.end(), newParticles.begin(), newParticles.end());
				}
			});
		m_blobs.clear();

		m_mainBlob = std::make_shared<Blob>(coreParticle, particles, linkElasticity, linkLength);
		m_blobs.push_back(m_mainBlob);
	}
}

std::shared_ptr<Blob> GameWorld::createBlob(const unsigned int blobCount)
{
	auto coreParticle = std::make_shared<physicslib::Particle>
		(1, physicslib::Vector3(400, 400, 0), physicslib::Vector3(0, 0, 0), physicslib::Vector3());
	m_particles.push_back(coreParticle);

	std::vector<std::shared_ptr<physicslib::Particle>> particles;
	for (unsigned int count = 0; count < blobCount - 1; ++count)
	{
		auto particle = std::make_shared<physicslib::Particle>
			(1, physicslib::Vector3(100+20*count, 100+20*count, 0), physicslib::Vector3(0, 0, 0), physicslib::Vector3());
		particles.push_back(particle);
		m_particles.push_back(particle);
	}

	std::shared_ptr<Blob> blob = std::make_shared<Blob>(coreParticle, particles, 4, 50);
	m_blobs.push_back(blob);
	return blob;
}

void GameWorld::separateMainBlob()
{
	// we cut the main blob in 2 blob
	std::shared_ptr<physicslib::Particle > coreParticle = m_mainBlob->getCore();
	std::vector<std::shared_ptr<physicslib::Particle>> particles = m_mainBlob->getParticles();
	std::shared_ptr<physicslib::Particle> secondCoreParticle = particles.back();
	particles.pop_back();
	std::vector<std::shared_ptr<physicslib::Particle>> secondParticles;

	std::vector<std::shared_ptr<physicslib::Particle>>::iterator particleIt = particles.begin();
	bool transfer = false;
	while (particleIt != particles.end())
	{
		if (transfer)
		{
			std::shared_ptr<physicslib::Particle> transferedParticle = *particleIt;
			particleIt = particles.erase(particleIt);
			secondParticles.push_back(transferedParticle);

			transferedParticle->setSpeed(physicslib::Vector3(-400, -400, 0)); // impulsion to see the separation of the blob
		}
		else
		{
			(*particleIt)->setSpeed(physicslib::Vector3(400, 400, 0)); // impulsion to see the separation of the blob
			++particleIt;
		}
		transfer = !transfer;
	}

	// deleting the old blob from m_blobs
	for (auto i = m_blobs.begin(); i != m_blobs.end(); ++i)
	{
		if ((*i) == m_mainBlob)
		{
			m_blobs.erase(i);
			break;
		}
	}

	// adding the new blobs in m_blobs
	std::shared_ptr<Blob> firstNewBlob = std::make_shared<Blob>(coreParticle, particles,
		m_mainBlob->getLinksElasticity(), m_mainBlob->getLinksRestingLength());
	std::shared_ptr<Blob> secondNewBlob = std::make_shared<Blob>(secondCoreParticle, secondParticles,
		m_mainBlob->getLinksElasticity(), m_mainBlob->getLinksRestingLength());
	m_mainBlob = firstNewBlob;
	m_blobs.push_back(firstNewBlob);
	m_blobs.push_back(secondNewBlob);
}

void GameWorld::updatePhysics(const double frametime)
{
	// Generates all forces and add them in the force register
	generateAllForces();

	// applies the forces inside the force register
	m_forceRegister.updateAllForces(frametime);

	// update the position of the particles
	updateParticlesPosition(frametime);

	// look for collisions and resolve them
	detectCollision();
	m_contactRegister.resolveContacts(frametime);

	// clean registers
	m_contactRegister.clear();
	m_forceRegister.clear();
}

void GameWorld::generateAllForces()
{
	generateGravityAndDragForces();
	generateBlobsForces();
	generateBuoyancyForces();
}

void GameWorld::generateGravityAndDragForces()
{
	std::for_each(m_particles.begin(), m_particles.end(),
		[this](const auto& particle)
		{
			m_forceRegister.add(physicslib::ForceRegister::ForceRecord(particle, gravityGenerator));
			m_forceRegister.add(physicslib::ForceRegister::ForceRecord(particle, dragGenerator));
		});
}


void GameWorld::generateBuoyancyForces()
{
	std::for_each(m_particles.begin(), m_particles.end(),
		[this](const auto& particle)
		{
			double x = particle->getPosition().getX();
			double y = particle->getPosition().getY();
			if (x > WATER_LIMIT && y - physicslib::Particle::PARTICLE_RADIUS < FLOOR_LEVEL)
			{
				// buoyancy force
				double particleVolume = pow(physicslib::Particle::PARTICLE_RADIUS * 2, 3);
				auto buoyancyForceGenerator = std::make_shared<physicslib::ParticleBuoyancyForceGenerator>
					(0, particleVolume, FLOOR_LEVEL, 10);
				physicslib::ForceRegister::ForceRecord record(particle, buoyancyForceGenerator);
				m_forceRegister.add(record);

				// drag force because water has more drag power than the air
				auto dragForceGenerator = std::make_shared<physicslib::DragForceGenerator>
					(0.47, 0);
				physicslib::ForceRegister::ForceRecord dragRecord(particle, dragForceGenerator);
				m_forceRegister.add(dragRecord);
			}
		});
}

void GameWorld::generateBlobsForces()
{
	if (!m_blobs.empty())
	{
		std::for_each(m_blobs.begin(), m_blobs.end(),
			[this](const auto& blob)
			{
				auto records = blob->getForceRecords();
				std::for_each(records.begin(), records.end(),
					[this](const auto& record)
					{
						m_forceRegister.add(record);
					});

				auto contactGenerators = blob->getParticleContacts();
				std::for_each(contactGenerators.begin(), contactGenerators.end(),
					[this](auto& contactGenerator)
					{
						contactGenerator.addContact(m_contactRegister);
					});
			});
	}
}

void GameWorld::detectCollision()
{
	auto particle1 = std::begin(m_particles);
	while (particle1 != std::end(m_particles))
	{
		auto particle2 = std::begin(m_particles);
		while (particle2 != particle1)
		{
			if ((*particle1)->isInContactWith(*(particle2->get())))
			{ //collision with another particle
				physicslib::Vector3 contactNormal = ((*particle1)->getPosition() - (*particle2)->getPosition()).getNormalizedVector();
				double vs = contactNormal * ((*particle1)->getSpeed() - (*particle2)->getSpeed());
				double penetration = 2 * physicslib::Particle::PARTICLE_RADIUS - ((*particle1)->getPosition() - (*particle2)->getPosition()).getNorm();
				physicslib::ParticleContact particleContact = physicslib::ParticleContact(particle1->get(), particle2->get(), 0.7, vs, penetration, contactNormal);
				m_contactRegister.add(particleContact);
			}

			particle2++;
		}
		if ((*particle1)->getPosition().getY() < FLOOR_LEVEL + physicslib::Particle::PARTICLE_RADIUS
			&& (*particle1)->getPosition().getX() < WATER_LIMIT)
		{  //collision on the ground
			physicslib::Vector3 contactNormal(0, 1, 0);
			double vs = contactNormal * (*particle1)->getSpeed();
			double penetration = 2 * physicslib::Particle::PARTICLE_RADIUS - ((*particle1)->getPosition() - physicslib::Vector3((*particle1)->getPosition().getX(), FLOOR_LEVEL, 0)).getNorm();
			(*particle1)->setPosition(physicslib::Vector3((*particle1)->getPosition().getX(), FLOOR_LEVEL, 0));
			physicslib::ParticleContact particleContact = physicslib::ParticleContact(particle1->get(), nullptr, 1.3, vs, penetration, contactNormal);
			m_contactRegister.add(particleContact);
		}
		if ((*particle1)->getPosition().getY() > CEILING_LEVEL - physicslib::Particle::PARTICLE_RADIUS)
		{//collision on the ceiling
			physicslib::Vector3 contactNormal(0, -1, 0);
			double vs = contactNormal * (*particle1)->getSpeed();
			double penetration = 2 * physicslib::Particle::PARTICLE_RADIUS - ((*particle1)->getPosition() - physicslib::Vector3((*particle1)->getPosition().getX(), CEILING_LEVEL, 0)).getNorm();
			(*particle1)->setPosition(physicslib::Vector3((*particle1)->getPosition().getX(), CEILING_LEVEL, 0));
			physicslib::ParticleContact particleContact = physicslib::ParticleContact(particle1->get(), nullptr, 1.0, vs, penetration, contactNormal);
			m_contactRegister.add(particleContact);
		}
		if ((*particle1)->getPosition().getX() < LEFT_WALL_LIMIT + physicslib::Particle::PARTICLE_RADIUS)
		{//collision on the left
			physicslib::Vector3 contactNormal(1, 0, 0);
			double vs = contactNormal * (*particle1)->getSpeed();
			double penetration = 2 * physicslib::Particle::PARTICLE_RADIUS - ((*particle1)->getPosition() - physicslib::Vector3(LEFT_WALL_LIMIT, (*particle1)->getPosition().getY(), 0)).getNorm();
			(*particle1)->setPosition(physicslib::Vector3(LEFT_WALL_LIMIT, (*particle1)->getPosition().getY(), 0));
			physicslib::ParticleContact particleContact = physicslib::ParticleContact(particle1->get(), nullptr, 1.3, vs, penetration, contactNormal);
			m_contactRegister.add(particleContact);
		}
		if ((*particle1)->getPosition().getX() > RIGHT_WALL_LIMIT - physicslib::Particle::PARTICLE_RADIUS)
		{//collision on the right
			physicslib::Vector3 contactNormal(-1, 0, 0);
			double vs = contactNormal * (*particle1)->getSpeed();
			double penetration = 2 * physicslib::Particle::PARTICLE_RADIUS - ((*particle1)->getPosition() - physicslib::Vector3(RIGHT_WALL_LIMIT, (*particle1)->getPosition().getY(), 0)).getNorm();
			(*particle1)->setPosition(physicslib::Vector3(RIGHT_WALL_LIMIT, (*particle1)->getPosition().getY(), 0));
			physicslib::ParticleContact particleContact = physicslib::ParticleContact(particle1->get(), nullptr, 1.3, vs, penetration, contactNormal);
			m_contactRegister.add(particleContact);
		}
		particle1++;
	}
}

void GameWorld::updateParticlesPosition(const double frametime)
{
	auto particle = std::begin(m_particles);
	while (particle != std::end(m_particles))
	{
		(*particle)->integrate(frametime); //use last frame time for integration
		if (!(*particle)->isVisible(SCR_WIDTH, SCR_HEIGHT))
		{
			particle = m_particles.erase(particle);
		}
		else
		{
			++particle;
		}
	}
}

void GameWorld::renderGame() const
{
	// cleaning screen
	m_openGlWrapper.clearCurrentWindow();

	// drawings
	drawBackground();
	drawParticles();

	// swapping the double buffers
	m_openGlWrapper.swapGraphicalBuffers(m_mainWindow);
}

void GameWorld::drawBackground() const
{
	opengl_wrapper::Shader backgroundShader = m_shaderPrograms.at(ShaderProgramType::BACKGROUND);
	backgroundShader.use();
	auto buffers = generateBackgroundBuffers();
	std::vector<double> verticesBuffer = std::get<0>(buffers);
	std::vector<unsigned int> indicesBuffer = std::get<1>(buffers);
	auto openglBuffers = m_openGlWrapper.createAndBindBackgroundDataBuffers(verticesBuffer, indicesBuffer);
	m_openGlWrapper.draw(GL_TRIANGLES, static_cast<unsigned int>(indicesBuffer.size()));
	m_openGlWrapper.cleanAndDeleteDataBuffers(openglBuffers);
}

void GameWorld::drawParticles() const
{
	opengl_wrapper::Shader particleShader = m_shaderPrograms.at(ShaderProgramType::PARTICLE);
	particleShader.use();
	auto buffers = generateParticlesBuffers();
	std::vector<double> verticesBuffer = std::get<0>(buffers);
	std::vector<unsigned int> indicesBuffer = std::get<1>(buffers);
	auto openglBuffers = m_openGlWrapper.createAndBindParticlesDataBuffers(verticesBuffer, indicesBuffer);
	particleShader.setUniform("circleRadius", static_cast<float>(physicslib::Particle::PARTICLE_RADIUS));
	m_openGlWrapper.draw(GL_TRIANGLES, static_cast<unsigned int>(indicesBuffer.size()));
	m_openGlWrapper.cleanAndDeleteDataBuffers(openglBuffers);
}

std::tuple<std::vector<double>, std::vector<unsigned int>> GameWorld::generateParticlesBuffers() const
{
	std::vector<double> vertices;
	std::vector<unsigned int> indices;

	unsigned int startIndex = 0;
	for (auto particle : m_particles)
	{
		double x = particle->getPosition().getX();
		double y = particle->getPosition().getY();
		double red = particle->getColor().getX();
		double green = particle->getColor().getY();
		double blue = particle->getColor().getZ();

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
		vertices.push_back(red);
		vertices.push_back(green);
		vertices.push_back(blue);

		// bottom right
		vertices.push_back(topX);
		vertices.push_back(bottomY);
		vertices.push_back(0);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0);
		vertices.push_back(red);
		vertices.push_back(green);
		vertices.push_back(blue);

		// bottom left
		vertices.push_back(bottomX);
		vertices.push_back(bottomY);
		vertices.push_back(0);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0);
		vertices.push_back(red);
		vertices.push_back(green);
		vertices.push_back(blue);

		// top left
		vertices.push_back(bottomX);
		vertices.push_back(topY);
		vertices.push_back(0);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0);
		vertices.push_back(red);
		vertices.push_back(green);
		vertices.push_back(blue);

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

std::tuple<std::vector<double>, std::vector<unsigned int>> GameWorld::generateBackgroundBuffers() const
{
	std::vector<double> vertices;
	std::vector<unsigned int> indices;

	//floor
	vertices.push_back(0);
	vertices.push_back(FLOOR_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);
	vertices.push_back(0);

	vertices.push_back(WATER_LIMIT);
	vertices.push_back(FLOOR_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);
	vertices.push_back(0);

	vertices.push_back(WATER_LIMIT);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);
	vertices.push_back(0);

	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);
	vertices.push_back(0);

	// right wall
	vertices.push_back(RIGHT_WALL_LIMIT);
	vertices.push_back(CEILING_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	vertices.push_back(SCR_WIDTH);
	vertices.push_back(CEILING_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	vertices.push_back(SCR_WIDTH);
	vertices.push_back(FLOOR_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	vertices.push_back(RIGHT_WALL_LIMIT);
	vertices.push_back(FLOOR_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	// left wall
	vertices.push_back(0);
	vertices.push_back(CEILING_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	vertices.push_back(LEFT_WALL_LIMIT);
	vertices.push_back(CEILING_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	vertices.push_back(LEFT_WALL_LIMIT);
	vertices.push_back(FLOOR_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	vertices.push_back(0);
	vertices.push_back(FLOOR_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	// ceilling
	vertices.push_back(0);
	vertices.push_back(SCR_HEIGHT);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	vertices.push_back(SCR_WIDTH);
	vertices.push_back(SCR_HEIGHT);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	vertices.push_back(SCR_WIDTH);
	vertices.push_back(CEILING_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	vertices.push_back(0);
	vertices.push_back(CEILING_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0.34);
	vertices.push_back(0.16);
	vertices.push_back(0);

	// water
	vertices.push_back(WATER_LIMIT);
	vertices.push_back(FLOOR_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);

	vertices.push_back(SCR_WIDTH);
	vertices.push_back(FLOOR_LEVEL);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);

	vertices.push_back(SCR_WIDTH);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);

	vertices.push_back(WATER_LIMIT);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);

	// floor
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// right wall
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(7);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	// left wall
	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(11);
	indices.push_back(9);
	indices.push_back(10);
	indices.push_back(11);

	// ceiling
	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(15);
	indices.push_back(13);
	indices.push_back(14);
	indices.push_back(15);

	// water
	indices.push_back(16);
	indices.push_back(17);
	indices.push_back(19);
	indices.push_back(17);
	indices.push_back(18);
	indices.push_back(19);

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
		inputsManager->addIntention(InputsManager::Intention::CREATE_BLOB);
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		inputsManager->addIntention(InputsManager::Intention::CREATE_PARTICLE_TWO);
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		inputsManager->addIntention(InputsManager::Intention::CREATE_PARTICLE_THREE);
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		inputsManager->addIntention(InputsManager::Intention::CUT_MAIN_BLOB);
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		inputsManager->addIntention(InputsManager::Intention::FUSION_ALL_BLOB);
	}

}
