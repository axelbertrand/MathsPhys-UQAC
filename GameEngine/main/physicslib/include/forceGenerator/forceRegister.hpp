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
			ForceRecord(std::shared_ptr<Particle> particle, std::shared_ptr<ParticleForceGenerator> forceGenerator);

			std::shared_ptr<Particle> particle;
			std::shared_ptr<ParticleForceGenerator> forceGenerator;
		};
		void add(const ForceRecord& record);
		void clear();

		void updateAllForces(double duration);
	private:
		std::vector<ForceRecord> m_register;
	};
}