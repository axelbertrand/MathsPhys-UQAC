#pragma once

#include <vector>
#include <tuple>

#include "particle.hpp"
#include "collisions/particleCable.hpp"
#include "forceGenerator/forceRegister.hpp"

class Blob
{
public:
	/* Warning, the core particle must no be in the given particles vector*/
	Blob(std::shared_ptr<physicslib::Particle> coreParticle, std::vector<std::shared_ptr<physicslib::Particle>> particles,
		double linksElasticity, double linksRestingLength);

	std::shared_ptr<physicslib::Particle> getCore();
	std::vector<std::shared_ptr<physicslib::Particle>> getParticles() const;
	std::vector<physicslib::ForceRegister::ForceRecord> getForceRecords() const;
	double getLinksElasticity() const;
	double getLinksRestingLength() const;
	std::vector<physicslib::ParticleCable> getParticleContacts() const;

private:
	const std::shared_ptr<physicslib::Particle> m_coreParticle;
	std::vector<std::shared_ptr<physicslib::Particle>> m_particles;
	std::vector<std::pair<const std::shared_ptr<physicslib::Particle>, const std::shared_ptr<physicslib::Particle>>> m_links;

	const double m_linksElasticity;
	const double m_linksRestingLength;
};
