#pragma once

#include "particleForceGenerator.hpp"

#include <vector>
#include <memory>

namespace physicslib
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
		void remove(Particle* particle);

		void updateForce(Particle* particle, double duration);

	private:
		std::vector<ForceRecord> m_register;
	};
}