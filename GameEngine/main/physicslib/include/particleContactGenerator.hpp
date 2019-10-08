#pragma once
#include <vector3.hpp>
#include <string>
#include "particle.hpp"

namespace physicslib
{
	class ParticleContactGenerator
	{

	public:
		ParticleContactGenerator() = default;
		virtual ~ParticleContactGenerator() = default;

		virtual void ajouterContact() = 0;
	};

}
