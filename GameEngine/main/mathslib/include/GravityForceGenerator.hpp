#include "vector3.hpp"
#include "Particle.hpp"
#include "ParticleForceGenerator.hpp"

namespace mathslib
{
	class GravityForceGenerator : public ParticleForceGenerator
	{
		public:
			GravityForceGenerator(Vector3 gravity);
			virtual ~GravityForceGenerator();

			void updateForce(Particle& particle, float duration) override;

		private:
			Vector3 m_gravity;
	};
}