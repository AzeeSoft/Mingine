#include "UIManager.h"
#include "src/asset.h"
#include <string>
#include <SDL.h>


namespace azee
{
	UIManager::UIManager(mingine::NativeGameLoop& native_game_loop)
		: GameObject(native_game_loop)
	{
	}

	void UIManager::start()
	{
		mingine::LoadParameters loadParameters;
		loadParameters.path = "fonts/8_bit_pusab.ttf";
		loadParameters.size = 22;
		loadParameters.assetType = mingine::AssetFont;

		std::string errorMessage("");
		fontId = assetDatabase.add(loadParameters, errorMessage);

		if (errorMessage != "")
		{
			SDL_Log(errorMessage.c_str());
		}

	}

	void UIManager::update()
	{
	}

	void UIManager::draw()
	{
		mingine::Font* font = assetDatabase.get<mingine::Font>(fontId);
//		font->draw("Boxes Left: ", 0, 0, 255,255,255);
	}

	UIManager::~UIManager()
	{
	}
}

