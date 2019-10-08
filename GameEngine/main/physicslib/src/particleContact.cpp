#include "../include/particleContact.hpp"
#include <algorithm>

namespace physicslib
{
	ParticleContact::ParticleContact(Particle* particle1, Particle* particle2, double restitution) :
		m_restitution(restitution)
	{
		m_particles[0] = particle1;
		m_particles[1] = particle2;
	}

	void ParticleContact::resolve(double frametime)
	{
		
		calculateVariables();
		//if (m_vs > 0)
		{
			resolveVelocity(frametime);
		}
		resolveInterpenetration();
	}

	void ParticleContact::calculateVariables()
	{
		m_contactNormal = (m_particles[0]->getSpeed() - m_particles[1]->getSpeed()).getNormalizedVector();
		m_vs = m_contactNormal * (m_particles[0]->getSpeed() - m_particles[1]->getSpeed());
		m_penetration = 2 * Particle::PARTICLE_RADIUS - (m_particles[0]->getPosition() - m_particles[1]->getPosition()).getNorm();
		if (m_penetration < 0)
		{
			m_penetration = 0;
		}
	}

	void ParticleContact::resolveInterpenetration()
	{
		if (m_particles[1]->getInverseMass() == 0 && m_particles[0]->getInverseMass() != 0)
		{
			Vector3 move1 = m_particles[0]->getPosition() - m_contactNormal * m_penetration;
			m_particles[0]->setPosition(move1);
		}
		else if (m_particles[0]->getInverseMass() == 0 && m_particles[1]->getInverseMass() != 0)
		{
			Vector3 move2 = m_particles[1]->getPosition() - m_contactNormal * m_penetration;
			m_particles[1]->setPosition(move2);
		}
		else if (m_particles[0]->getInverseMass() != 0 && m_particles[1]->getInverseMass() != 0)
		{
			Vector3 move1 = m_particles[0]->getPosition() + m_contactNormal * m_penetration * (1 / m_particles[1]->getInverseMass()) / (1 / m_particles[1]->getInverseMass() + 1 / m_particles[0]->getInverseMass());
			m_particles[0]->setPosition(move1);
			Vector3 move2 = m_particles[1]->getPosition() - m_contactNormal * m_penetration * (1 / m_particles[0]->getInverseMass()) / (1 / m_particles[1]->getInverseMass() + 1 / m_particles[0]->getInverseMass());
			m_particles[1]->setPosition(move2);
		}
	}
		

	void ParticleContact::resolveVelocity(double frametime)
	{
		Vector3 newVelocity1 = m_particles[0]->getSpeed() + m_contactNormal * m_vs * m_particles[0]->getInverseMass() * m_restitution * frametime;
		m_particles[0]->setSpeed(newVelocity1);
		Vector3 newVelocity2 = m_particles[1]->getSpeed() - m_contactNormal * m_vs * m_particles[1]->getInverseMass() * m_restitution * frametime;
		m_particles[1]->setSpeed(newVelocity2);
	}

	ParticleContact::~ParticleContact()
	{

	}

}