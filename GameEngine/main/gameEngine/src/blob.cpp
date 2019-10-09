#include "../include/blob.hpp"

#include <algorithm>

#include "forceGenerator/forceRegister.hpp"
#include "forceGenerator/particleSpringForceGenerator.hpp"


Blob::Blob(std::shared_ptr<physicslib::Particle> coreParticle, std::vector<std::shared_ptr<physicslib::Particle>> particles,
	double linksElasticity, double linksRestingLength):
	m_coreParticle(coreParticle), m_particles(particles),
	m_linksElasticity(linksElasticity), m_linksRestingLength(linksRestingLength)
{
	std::for_each(m_particles.begin(), m_particles.end(),
		[this](const auto particle)
		{
			m_links.push_back(std::pair<const std::shared_ptr<physicslib::Particle>,
				const std::shared_ptr<physicslib::Particle>>(m_coreParticle, particle));
		});
}

std::shared_ptr<physicslib::Particle> Blob::getCore() const
{
	return m_coreParticle;
}

std::vector<physicslib::ForceRegister::ForceRecord> Blob::getForceRecords() const
{
	std::vector<physicslib::ForceRegister::ForceRecord> res;
	std::for_each(m_links.begin(), m_links.end(),
		[&res,this](const std::pair<const std::shared_ptr<physicslib::Particle>, const std::shared_ptr<physicslib::Particle>> link)
		{
			physicslib::ForceRegister::ForceRecord record1(link.first,
				std::make_shared<const physicslib::ParticleSpringForceGenerator>
				(physicslib::ParticleSpringForceGenerator(link.second, m_linksElasticity, m_linksRestingLength)));
			physicslib::ForceRegister::ForceRecord record2(link.second,
				std::make_shared<const physicslib::ParticleSpringForceGenerator>
				(physicslib::ParticleSpringForceGenerator(link.first, m_linksElasticity, m_linksRestingLength)));
			res.push_back(record1);
			res.push_back(record2);
		});
	return res;
}

