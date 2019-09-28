#include "GravityForceGenerator.hpp"

namespace mathslib
{
	GravityForceGenerator::GravityForceGenerator(Vector3 gravity)
		: m_gravity(gravity)
	{
	}

	GravityForceGenerator::~GravityForceGenerator()
	{
	}

	void GravityForceGenerator::updateForce(Particle* particle, float duration)
	{
		if (particle->getInverseMass() != 0)
		{
			particle->addForce(m_gravity * particle->getInverseMass());
		}
	}
}