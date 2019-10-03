#pragma once

#include "particleForceGenerator.hpp"
#include "vector3.hpp"

namespace physicslib
{
	class BungeeSpringForceGenerator : public ParticleForceGenerator
	{
	public:
		BungeeSpringForceGenerator(Particle otherParticle, double elasticity, double restingLength);
		virtual ~BungeeSpringForceGenerator();

		void updateForce(Particle* particle, double duration) override;

	private:
		Particle m_otherParticle;
		double m_elasticity;
		double m_restingLength;
	};
}