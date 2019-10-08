#include "collisions/particleCable.hpp"

namespace physicslib
{
	ParticleCable::ParticleCable(Particle particle1, Particle particle2, double maxLength, double restitutionCoef) :
		ParticleLink(particle1, particle2), m_maxLength(maxLength), m_restitutionCoef(restitutionCoef)
	{
	}

	void ParticleLink::addContact()
	{

	}
}