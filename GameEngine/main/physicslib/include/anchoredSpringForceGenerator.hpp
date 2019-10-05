#pragma once

#include "particleForceGenerator.hpp"
#include "vector3.hpp"

namespace physicslib
{
	class AnchoredSpringForceGenerator : public ParticleForceGenerator
	{
	public:
		AnchoredSpringForceGenerator(Vector3 anchorPosition, double elasticity, double restingLength);
		virtual ~AnchoredSpringForceGenerator();

		void updateForce(Particle* particle, double duration) override;

	private:
		Vector3 m_anchorPosition;
		double m_elasticity;
		double m_restingLength;
	};
}