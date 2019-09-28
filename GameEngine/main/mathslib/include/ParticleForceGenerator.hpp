#pragma once

#include "../../myexe/include/Particle.hpp"

namespace mathslib
{
	class ParticleForceGenerator
	{
		public:
			ParticleForceGenerator() = default;
			virtual ~ParticleForceGenerator() = default;

			virtual void updateForce(Particle* particle, double duration) = 0;
	};
}