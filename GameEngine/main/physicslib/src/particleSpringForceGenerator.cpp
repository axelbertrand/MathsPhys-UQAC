#include "particleSpringForceGenerator.hpp"

namespace physicslib 
{
	ParticleSpringForceGenerator::ParticleSpringForceGenerator(Particle otherParticle,
		double elasticity, double restingLength)
		: m_otherParticle(otherParticle), m_elasticity(elasticity), m_restingLength(restingLength)
	{}

	ParticleSpringForceGenerator::~ParticleSpringForceGenerator() 
	{}

	void ParticleSpringForceGenerator::updateForce(Particle* particle, double duration)
	{
		Vector3 d = particle->getPosition() - m_otherParticle.getPosition();
		particle->addForce(d.getNormalizedVector() * (-m_elasticity) * (d.getNorm() - m_restingLength));
		m_otherParticle.addForce(d.getNormalizedVector() * (m_elasticity) * (d.getNorm() - m_restingLength)); /* TODO A verifier */
	}
}