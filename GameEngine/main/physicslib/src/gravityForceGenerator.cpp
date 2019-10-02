#include "gravityForceGenerator.hpp"

namespace physicslib
{
	GravityForceGenerator::GravityForceGenerator(Vector3 gravity)
		: m_gravity(gravity)
	{
	}

	GravityForceGenerator::~GravityForceGenerator()
	{
	}

	void GravityForceGenerator::updateForce(Particle* particle, double duration)
	{
		if (particle->getInverseMass() != 0)
		{
			particle->addForce(m_gravity);
		}
	}
}