#pragma once

#include "particleForceGenerator.hpp"
#include "vector3.hpp"

namespace physicslib
{
	class ParticleSpringForceGenerator : public ParticleForceGenerator
	{
	public:
		ParticleSpringForceGenerator(Particle otherParticle, double elasticity, double restingLength);
		virtual ~ParticleSpringForceGenerator();

		void updateForce(Particle* particle, double duration) override;

	private:
		Particle m_otherParticle;
		double m_elasticity;
		double m_restingLength;
	};
}

