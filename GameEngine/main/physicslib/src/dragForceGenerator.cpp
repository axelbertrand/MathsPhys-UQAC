#include "dragForceGenerator.hpp"

namespace physicslib
{
	DragForceGenerator::DragForceGenerator(double k1, double k2)
		: m_k1(k1)
		, m_k2(k2)
	{
	}

	DragForceGenerator::~DragForceGenerator()
	{
	}

	void DragForceGenerator::updateForce(Particle* particle, double duration)
	{
		double speedNorm = particle->getSpeed().getNorm();
		double squaredSpeedNorm = particle->getSpeed().getSquaredNorm();
		Vector3 normalizedSpeed = particle->getSpeed().getNormalizedVector();

		Vector3 dragForce = normalizedSpeed * (m_k1 * speedNorm + m_k2 * squaredSpeedNorm) * -1;

		particle->addForce(dragForce);
	}
}