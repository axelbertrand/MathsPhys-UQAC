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

	void ForceRegister::remove(Particle* particle)
	{
		auto last = std::remove_if(m_register.begin(), m_register.end(),
			[particle](ForceRecord& record)
			{
				return record.particle == particle;
			});

		m_register.erase(last, m_register.end());
	}

	void ForceRegister::updateForce(Particle* particle, double duration)
	{
		for (ForceRecord& record : m_register)
		{
			if (record.particle == particle)
			{
				record.forceGenerator->updateForce(particle, duration);
			}
		}
	}
}