#ifdef _WIN32
#define NOMINMAX
#endif

#include "../include/gameEngine.hpp"

int main(int argc, char* argv[])
{
	GameEngine gameEngine;
	gameEngine.run();
	return EXIT_SUCCESS;
}