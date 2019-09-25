#include "Particle.hpp"

namespace mathslib
{
	class ParticleForceGenerator
	{
		public:
			ParticleForceGenerator() = default;
			virtual ~ParticleForceGenerator() = default;

			virtual void updateForce(Particle* particle, float duration) = O;
	};
}