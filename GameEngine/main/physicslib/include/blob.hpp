#include <vector>
#include <tuple>
#include "particle.hpp"
#include "forceRegister.hpp"

namespace physicslib
{
	class Blob
	{
	public :
		Blob(std::shared_ptr<Particle> coreParticle, std::vector<std::shared_ptr<Particle>> particles,
			std::vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>> links,
			double linksElasticity, double linksRestingLength);
		~Blob();

		std::shared_ptr<Particle> getCore();

		std::vector<ForceRegister::ForceRecord> getForceRecords();
		void updateForceRegister(ForceRegister forceRegister);

	private :
		std::shared_ptr<Particle> m_coreParticle;
		std::vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>> m_links;
		std::vector<std::shared_ptr<Particle>> m_particles;
		double m_linksElasticity;
		double m_linksRestingLength;
	};
}