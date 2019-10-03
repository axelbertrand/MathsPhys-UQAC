#pragma once

#include "particleForceGenerator.hpp"
#include "vector3.hpp"

namespace physicslib
{
	class DragForceGenerator : public ParticleForceGenerator
	{
	public:
		DragForceGenerator(double k1, double k2);
		virtual ~DragForceGenerator();

		void updateForce(Particle* particle, double duration) override;

	private:
		const double m_k1;
		const double m_k2;
	};
}