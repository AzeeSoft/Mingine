#include "GameMath.h"
#include <cstdlib>
#include <time.h>

namespace mingine
{
	GameMath::GameMath()
	{
	}


	GameMath::~GameMath()
	{
	}

	int GameMath::getRandomBetween(int min, int max)
	{
		static bool seeded = false;
		if(!seeded)
		{
			srand(time(nullptr)); //seeding for the first time only!
			seeded = true;
		}

		return min + rand() % ((max + 1) - min);
	}
}
