#pragma once

#include "particle.hpp"

namespace physicslib
{
	class ParticleForceGenerator
	{
	public:
		ParticleForceGenerator() = default;
		virtual ~ParticleForceGenerator() = default;

		virtual void updateForce(Particle* particle, double duration) = 0;
	};
}