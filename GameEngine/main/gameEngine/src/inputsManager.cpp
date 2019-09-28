#include "../include/inputsManager.hpp"

std::vector<InputsManager::Intention> InputsManager::getPendingIntentions() const
{
	return m_pendingIntentions;
}

void InputsManager::clearIntentions()
{
	m_pendingIntentions.clear();
}

void InputsManager::addIntention(Intention intention)
{
	m_pendingIntentions.push_back(intention);
}