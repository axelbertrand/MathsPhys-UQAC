#include "Particle.hpp"
#include "ParticleForceGenerator.hpp"

#include <vector>

namespace mathslib
{
	class ForceRegister
	{
	public:
		struct ForceRecord
		{
			Particle* particle;
			ParticleForceGenerator* forceGenerator;
		};

		void add(const ForceRecord& record);
		void remove(const ForceRecord& record);
		ForceRecord get(unsigned int record) const;

	private:
		std::vector<ForceRecord> m_register;
	};
}
