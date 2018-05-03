#pragma once
#include "GameObject.h"
#include "src/assetDatabase.h"

namespace azee
{
	class UIManager : mingine::GameObject
	{
	public:
		UIManager(mingine::NativeGameLoop& native_game_loop);
		~UIManager();

		void start() override;
		void update() override;
		void draw() override;

	private:
		mingine::AssetDatabase assetDatabase;
		int fontId;
	};
}

