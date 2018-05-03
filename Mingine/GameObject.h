#pragma once
#include "src/nativeGameLoop.h"

namespace mingine
{
	class GameObject
	{
	public:
		GameObject(NativeGameLoop& native_game_loop);
		virtual ~GameObject();

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;

	protected:
		NativeGameLoop& getNativeGameLoop() const;

	private:
		NativeGameLoop& nativeGameLoop;
	};
}

