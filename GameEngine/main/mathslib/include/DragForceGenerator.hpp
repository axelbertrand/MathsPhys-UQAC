#pragma once

#include "ParticleForceGenerator.hpp"
#include "vector3.hpp"

namespace mathslib
{
	class DragForceGenerator : public ParticleForceGenerator
	{
		public:
			DragForceGenerator(double k1, double k2);
			virtual ~DragForceGenerator();

			void updateForce(Particle* particle, float duration) override;

		private:
			double m_k1;
			double m_k2;
	};
}