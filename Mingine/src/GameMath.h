#pragma once

namespace mingine 
{
	class GameMath
	{
	private:
		GameMath();
		~GameMath();

	public:
		static int getRandomBetween(int min, int max);
	};
}

