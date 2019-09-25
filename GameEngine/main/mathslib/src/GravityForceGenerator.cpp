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

	void GravityForceGenerator::updateForce(Particle& particle, float duration)
	{
		double mass = 1. / particle.getInverseMass();
		if (mass != 0)
		{
			particle.addForce(m_gravity * mass);
		}
	}
}