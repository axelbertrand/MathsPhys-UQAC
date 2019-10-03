#include "anchoredSpringForceGenerator.hpp"

namespace physicslib
{
	AnchoredSpringForceGenerator::AnchoredSpringForceGenerator
		(Vector3 anchorPosition, double elasticity, double restingLength) :
		m_anchorPosition(anchorPosition), m_elasticity(elasticity), m_restingLength(restingLength)
		{}

	AnchoredSpringForceGenerator::~AnchoredSpringForceGenerator()
	{}

	void AnchoredSpringForceGenerator::updateForce(Particle* particle, double duration)
	{
		Vector3 d = particle->getPosition() - m_anchorPosition;
		particle->addForce(d.getNormalizedVector() * (m_elasticity) * (d.getNorm() - m_restingLength));
	}
}