#include "blob.hpp"
#include "forceRegister.hpp"
#include "particleSpringForceGenerator.hpp"
#include <algorithm>

namespace physicslib
{
	Blob::Blob(std::shared_ptr<Particle> coreParticle, std::vector<std::shared_ptr<Particle>> particles, std::vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>> links
		, double linksElasticity, double linksRestingLength):
		m_coreParticle(coreParticle), m_particles(particles), m_links(links),
		m_linksElasticity(linksElasticity), m_linksRestingLength(linksRestingLength)
	{}
	Blob::~Blob()
	{}

	std::shared_ptr<Particle> Blob::getCore()
	{
		return m_coreParticle;
	}

	std::vector<ForceRegister::ForceRecord> Blob::getForceRecords()
	{
		std::vector<ForceRegister::ForceRecord> res;
		std::for_each(m_links.begin(), m_links.end(), [&res, this](std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>> link)
			{
				ForceRegister::ForceRecord record1 = ForceRegister::ForceRecordForceRecord(link.first, ParticleSpringForceGenerator(link.second, m_linksElasticity, m_linksRestingLength));
				ForceRegister::ForceRecord record2 = ForceRegister::ForceRecordForceRecord(link.second, ParticleSpringForceGenerator(link.first, m_linksElasticity, m_linksRestingLength));;
				res.push_back(record1);
				res.push_back(record2);
			});
		return res;
	}

	/*void Blob::updateForceRegister(ForceRegister forceRegister)
	{
		std::for_each(m_links.begin(), m_links.end(), [this,&forceRegister](std::pair<Particle, Particle> link)
			{
				ForceRegister::ForceRecord record1 = { link.first, ParticleSpringForceGenerator(link.second, m_linksElasticity, m_linksRestingLength) };
				ForceRegister::ForceRecord record2 = { link.second, ParticleSpringForceGenerator(link.first, m_linksElasticity, m_linksRestingLength) };
				forceRegister.add(record1);
			});
	}*/
}