#include "collisions/particleCable.hpp"

#include "collisions/particleContact.hpp"

namespace physicslib
{
	ParticleCable::ParticleCable(Particle particle1, Particle particle2, double maxLength, double restitutionCoef) :
		ParticleLink(particle1, particle2), m_maxLength(maxLength), m_restitutionCoef(restitutionCoef)
	{
	}

	ParticleCable::~ParticleCable()
	{
	}

	void ParticleCable::addContact(ContactRegister& contactRegister)
	{
		if (m_particles[0].isInContactWith(m_particles[1]) || getCurrentLength() >= m_maxLength)
		{
			contactRegister.add(ParticleContact(&m_particles[0], &m_particles[1], m_restitutionCoef));
		}
	}
}