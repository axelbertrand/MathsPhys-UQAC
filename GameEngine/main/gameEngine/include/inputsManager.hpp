#pragma once

#include <vector>

#include "OpenGlWrapper.hpp"

class InputsManager
{
public:
	enum Intention
	{
		CLOSE_MAIN_WINDOW,
		CREATE_BLOB,
		CREATE_PARTICLE_TWO,
		CREATE_PARTICLE_THREE,
		MOVE_BLOB_RIGHT,
		MOVE_BLOB_LEFT,
		MOVE_BLOB_TOP,
		MOVE_BLOB_BOTTOM
	};

	InputsManager() {};

	std::vector<Intention> getPendingIntentions(GLFWwindow* window);
	void clearIntentions();
	void addIntention(Intention intention);

private:
	std::vector<Intention> m_pendingIntentions;
};