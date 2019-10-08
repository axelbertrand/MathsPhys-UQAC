#include "forceRegister.hpp"

#include <algorithm>

namespace physicslib
{
	ForceRegister::ForceRecord::ForceRecord(std::shared_ptr<Particle> particle, std::shared_ptr<ParticleForceGenerator> forceGenerator)
		: particle(particle), forceGenerator(forceGenerator)
	{
	}

	void ForceRegister::add(const ForceRecord& record)
	{
		m_register.push_back(record);
	}

	void ForceRegister::remove(std::shared_ptr<Particle> particle)
	{
		auto last = std::remove_if(m_register.begin(), m_register.end(),
			[particle](const ForceRecord& record)
			{
				return record.particle == particle;
			});

		m_register.erase(last, m_register.end());
	}

	void ForceRegister::updateAllForces(double duration)
	{
		std::for_each(m_register.begin(), m_register.end(), 
			[duration](ForceRecord& record)
			{
				record.forceGenerator->updateForce(record.particle, duration);
			});
	}
}