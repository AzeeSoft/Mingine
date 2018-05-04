#pragma once
#include "platform.h"

namespace mingine
{

	class NativeGameLoop
	{
	public:
		NativeGameLoop();
		virtual ~NativeGameLoop();

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
	};
}

