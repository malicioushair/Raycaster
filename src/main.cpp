#include <SFML/Graphics.hpp>
#include <cstdlib>

#include "GameLoop.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv)
{
	Raycaster::GameLoop gameLoop;
	gameLoop.Run();

	return EXIT_SUCCESS;
}