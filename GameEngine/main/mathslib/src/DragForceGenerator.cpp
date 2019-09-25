#include "GravityForceGenerator.hpp"

namespace mathslib
{
	DragForceGenerator::DragForceGenerator(double k1, double k2)
		: m_k1(k1)
		, m_k2(k2)
	{
	}

	DragForceGenerator::~DragForceGenerator()
	{
	}

	void DragForceGenerator::updateForce(Particle& particle, float duration)
	{
		double speedNorm = particle.getSpeed().getNorm();
		double squaredSpeedNorm = particle.getSpeed().getSquaredNorm();
		Vector3 normalizedSpeed = particle.getSpeed().getNormalizedVector();

		double dragForce = -normalizedSpeed * (k1 * speedNorm + k2 * squaredSpeedNorm);

		particle.addForce(dragForce);
	}
}