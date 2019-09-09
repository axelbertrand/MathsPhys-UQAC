#include "..\include\GameEngine.hpp"
#include <vector3.hpp>
#include <iostream>
#include "../include/Particle.hpp"

int main()
{
	GameEngine gameEngine("Game Engine");

	gameEngine.run();

	return EXIT_SUCCESS;
}