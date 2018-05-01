#pragma once
#include "src/nativeGameLoop.h"

using namespace mingine;

namespace azee
{
	class FinalGame : public NativeGameLoop
	{
	public:
		FinalGame();
		~FinalGame();

		void start() override;
		void update() override;
		void draw() override;
	};
}

