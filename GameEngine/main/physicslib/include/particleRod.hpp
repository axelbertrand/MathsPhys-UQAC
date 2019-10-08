#pragma once

#include "particleLink.hpp"

namespace physicslib
{
	class ParticleRod : ParticleLink
	{
	public:
		ParticleRod(Particle particle1, Particle particle2, double length);
		virtual ~ParticleRod();
		void addContact();

	private:
		double m_length;
	};
}