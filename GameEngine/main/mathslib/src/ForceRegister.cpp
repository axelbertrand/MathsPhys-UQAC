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

	void ForceRegister::updateForce(Particle* particle, double duration)
	{
		auto found = std::find_if(m_register.begin(), m_register.end(), [&](const ForceRecord& record) {
			return record.particle == particle;
		});

		ParticleForceGenerator* forceGenerator = found->forceGenerator;
		forceGenerator->updateForce(particle, duration);
	}
}