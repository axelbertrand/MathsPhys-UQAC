#pragma once

#include <vector>

#include "OpenGlWrapper.hpp"

class InputsManager
{
public:
	enum Intention
	{
		CLOSE_MAIN_WINDOW,
		CREATE_PARTICLE_ONE,
		CREATE_PARTICLE_TWO,
		CREATE_PARTICLE_THREE,
		CREATE_TEST_BLOB
	};

	InputsManager() {};

	std::vector<Intention> getPendingIntentions() const;
	void clearIntentions();
	void addIntention(Intention intention);

private:
	std::vector<Intention> m_pendingIntentions;
};