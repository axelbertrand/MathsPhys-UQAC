#include "ForceRegister.hpp"

namespace mathslib
{
	void ForceRegister::add(const ForceRecord& record)
	{
		m_register.push_back(record);
	}

	void ForceRegister::remove(const ForceRecord& record)
	{
		auto found = find(m_register.begin(), m_register.end(), record);
		if (found != m_register.end())
		{
			m_register.erase(found);
		}
	}

	ForceRegister::ForceRecord ForceRegister::get(unsigned int record) const
	{
		return m_register.at(record);
	}
}