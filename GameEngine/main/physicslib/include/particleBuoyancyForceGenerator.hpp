#pragma once

#include "particleForceGenerator.hpp"
#include "vector3.hpp"

namespace physicslib 
{
	class ParticleBuoyancyForceGenerator : public ParticleForceGenerator
	{
	public :
		ParticleBuoyancyForceGenerator(double maxDepth, double objectVolume, double liquidHeight, double liquidDensity);
		virtual ~ParticleBuoyancyForceGenerator();

		void updateForce(Particle* particle, double duration) override;

	private:
		double m_maxDepth;
		double m_objectVolume;
		double m_liquidHeight;
		double m_liquidDensity;
	};
}