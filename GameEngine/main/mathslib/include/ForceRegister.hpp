#pragma once

#include "ParticleForceGenerator.hpp"

#include <vector>
#include <memory>

namespace mathslib
{
	class ForceRegister
	{
	public:
		struct ForceRecord
		{
			ForceRecord(Particle& particle, ParticleForceGenerator& forceGenerator);

			Particle* particle;
			ParticleForceGenerator* forceGenerator;
		};

		void add(ForceRecord& record);

		void updateForces(float duration);

	private:
		std::vector<ForceRecord> m_register;
	};
}
