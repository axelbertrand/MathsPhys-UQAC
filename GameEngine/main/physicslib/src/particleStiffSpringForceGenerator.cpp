#include "particleStiffSpringForceGenerator.hpp"


namespace physicslib
{
	ParticleStiffSpringForceGenerator::ParticleStiffSpringForceGenerator
	(Vector3 anchorPosition, double elasticity, double damping) :
		m_anchorPosition(anchorPosition), m_elasticity(elasticity), m_damping(damping)
	{}
		
	ParticleStiffSpringForceGenerator::~ParticleStiffSpringForceGenerator()
	{}

	ParticleStiffSpringForceGenerator::updateForce(Particle* particle, double duration)
	{
		Vector3 positionFromRest = particle->getPosition() - m_anchorPosition;
		particle->addForce(- positionFromRest * m_elasticity - particle->getSpeed() * m_damping); // TODO à revoir
	}
}