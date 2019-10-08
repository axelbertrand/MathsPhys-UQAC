#include "collisions/particleRod.hpp"

namespace physicslib
{
	ParticleRod::ParticleRod(Particle particle1, Particle particle2, double length) :
		ParticleLink(particle1, particle2), m_length(length)
	{
		m_particles[0] = particle1;
		m_particles[1] = particle2;
	}

	void ParticleLink::addContact()
	{

	}
}