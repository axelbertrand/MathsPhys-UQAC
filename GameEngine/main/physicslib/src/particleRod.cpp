#include "particleRod.hpp"

#include "particle.hpp"

namespace physicslib
{
	ParticleRod::ParticleRod(Particle particle1, Particle particle2, double length) :
		ParticleLink(particle1, particle2), m_length(length)
	{
	}

	ParticleRod::~ParticleRod()
	{
	}

	void ParticleRod::addContact(ContactRegister& contactRegister)
	{
		contactRegister.add(ParticleContact(&m_particles[0], &m_particles[1], 0.));
	}
}