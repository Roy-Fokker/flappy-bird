#include "flappy.h"


auto main() -> int
{
	auto flappy = flappy_bird_game::flappy_bird();

	flappy.run();

	return EXIT_SUCCESS;
}