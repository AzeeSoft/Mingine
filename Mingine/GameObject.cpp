#include "GameObject.h"


namespace mingine
{
	GameObject::GameObject(NativeGameLoop& native_game_loop)
		: nativeGameLoop(native_game_loop)
	{
	}

	GameObject::~GameObject()
	{
	}

	NativeGameLoop& GameObject::getNativeGameLoop() const
	{
		return nativeGameLoop;
	}
}

