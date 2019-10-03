#include "bungeeSpringForceGenerator.hpp"

namespace physicslib
{
	BungeeSpringForceGenerator::BungeeSpringForceGenerator
	(Particle otherParticle, double elasticity, double restingLength) :
		m_otherParticle(otherParticle), m_elasticity(elasticity), m_restingLength(restingLength)
	{}

	BungeeSpringForceGenerator::~BungeeSpringForceGenerator()
	{}

	void BungeeSpringForceGenerator::updateForce(Particle* particle, double duration)
	{
		Vector3 d = particle->getPosition() - m_otherParticle.getPosition();
		if (d.getNorm() >= m_restingLength)
		{
			particle->addForce(d.getNormalizedVector() * (m_elasticity) * (d.getNorm() - m_restingLength));
		}
	}
}