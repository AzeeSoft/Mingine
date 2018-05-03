#pragma once

#include "src/nativeGameLoop.h"
#include "SDL.h"
#include "Model.h"
#include <string>
#include "Camera.h"

using namespace mingine;

namespace azee
{
	class FinalGame : public NativeGameLoop
	{
	public:
		void start() override;
		void update() override;
		void draw() override;

		void applyCommonShaderValues(Shader& shader);
		Camera getCamera() const;

		static FinalGame* getInstance();
	private:
		FinalGame();
		~FinalGame();

		static FinalGame* finalGameInstance;

		SDL_Window* mainWindow;
		SDL_GLContext mainContext;

		Camera camera;
		Model model;

		glm::vec3 lightPos;

		void log(char* msg, int line = -1);
		bool setOpenGLAttributes();

		void clearBackground(float r, float g, float b);
		bool initGame();
	};
}

