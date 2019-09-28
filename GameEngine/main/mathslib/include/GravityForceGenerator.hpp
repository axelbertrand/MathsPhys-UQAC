#pragma once

#include "ParticleForceGenerator.hpp"
#include "vector3.hpp"

namespace mathslib
{
	class GravityForceGenerator : public ParticleForceGenerator
	{
		public:
			GravityForceGenerator(Vector3 gravity);
			virtual ~GravityForceGenerator();

			void updateForce(Particle* particle, double duration) override;

		private:
			Vector3 m_gravity;
	};
}