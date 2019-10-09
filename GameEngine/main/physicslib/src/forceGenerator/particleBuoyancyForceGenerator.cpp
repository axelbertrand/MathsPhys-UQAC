#include "forceGenerator/particleBuoyancyForceGenerator.hpp"

#include <math.h>
#include <iostream>

namespace physicslib
{
	ParticleBuoyancyForceGenerator::ParticleBuoyancyForceGenerator
		(double maxDepth, double objectVolume, double liquidHeight, double liquidDensity) :
		m_maxDepth(maxDepth), m_objectVolume(objectVolume), m_liquidHeight(liquidHeight), m_liquidDensity(liquidDensity)
	{}

	void ParticleBuoyancyForceGenerator::updateForce(std::shared_ptr<Particle> particle, const double duration) const
	{
		//the particle is aproximated by a cube of equal volume
		double cubeEdge = pow(m_objectVolume, 1 / 3);
		double submergedProportion = (particle->getPosition().getZ() - m_liquidHeight - cubeEdge/2.)/cubeEdge; // Doute, peut �tre l'oppos�
		Vector3 zeroVector(0,1,0);
		if (submergedProportion <= 1 && submergedProportion >= 0)
		{
	
			particle->addForce(zeroVector * submergedProportion * m_objectVolume * m_liquidDensity);
		}
		else if (submergedProportion >= 1)
		{
			particle->addForce(zeroVector * m_objectVolume * m_liquidDensity);
		}
	}
}