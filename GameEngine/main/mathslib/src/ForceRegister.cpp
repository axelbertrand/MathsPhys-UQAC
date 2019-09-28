#include "ForceRegister.hpp"

#include <algorithm>

namespace mathslib
{
	ForceRegister::ForceRecord::ForceRecord(Particle& particle, ParticleForceGenerator& forceGenerator)
		: particle(&particle)
		, forceGenerator(&forceGenerator)
	{
	}

	void ForceRegister::add(ForceRecord& record)
	{
		m_register.push_back(record);
	}

	void ForceRegister::updateForces(float duration)
	{
		for (ForceRecord& record : m_register)
		{
			record.forceGenerator->updateForce(record.particle, duration);
		}
	}
}