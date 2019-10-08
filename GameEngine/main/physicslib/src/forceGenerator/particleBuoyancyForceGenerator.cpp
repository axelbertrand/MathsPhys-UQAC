#include "forceGenerator/particleBuoyancyForceGenerator.hpp"

#include <math.h>

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
		double submergedProportion = (particle->getPosition().getZ() - m_liquidHeight - cubeEdge/2.)/cubeEdge; // Doute, peut être l'opposé
		if (submergedProportion <= 1 && submergedProportion >= 0)
		{
			particle->addForce(Vector3::Vector3(0,0,1) * submergedProportion * m_objectVolume * m_liquidDensity);
		}
		else if (submergedProportion >= 1)
		{
			particle->addForce(Vector3::Vector3(0, 0, 1) * m_objectVolume * m_liquidDensity);
		}
	}
}