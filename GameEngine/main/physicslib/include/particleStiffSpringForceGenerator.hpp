#pragma once

#include "particleForceGenerator.hpp"
#include "vector3.hpp"

namespace physicslib
{
	class ParticleStiffSpringForceGenerator : public ParticleForceGenerator
	{
	public:
		ParticleStiffSpringForceGenerator(Vector3 anchorPosition, double elasticity, double damping);
		virtual ~ParticleStiffSpringForceGenerator();

		void updateForce(Particle* particle, double duration) override;

	private:
		Vector3 m_anchorPosition;
		double m_elasticity;
		double m_damping;
	};
}