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

			std::shared_ptr<Particle> particle;
			std::shared_ptr<ParticleForceGenerator> forceGenerator;
		};
		void add(ForceRecord& record);
		void remove(Particle* particle);

		void updateAllForces(double duration);
	private:
		std::vector<ForceRecord> m_register;
	};
}