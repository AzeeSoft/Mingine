#pragma once

#include "src/nativeGameLoop.h"
#include "SDL.h"
#include "Cube.h"
#include <string>
#include "Camera.h"
#include <vector>
#include "Brush.h"
#include "UIManager.h"

using namespace mingine;

namespace azee
{
	class FinalGame : public NativeGameLoop
	{
	public:
		void start() override;
		void update() override;
		void draw() override;

		void applyCommonFixedShaderValues(Shader& shader);
		void applyCommonDynamicShaderValues(Shader& shader);
		int getScreenWidth() const;
		int getScreenHeight() const;
		Camera getCamera() const;

		static FinalGame* getInstance();
	private:
		FinalGame();
		~FinalGame();

		static FinalGame* finalGameInstance;

		// To skip the first update after showing the message box (otherwise, the deltaTime is calculated from the last update, which would be huge)
		bool skipUpdate = false;

		int screenWidth = 1024;
		int screenHeight = 768;

		float elapsedTime = 0;
		float resetTime = 30;

		float maxBound = 10.0f;

		int cubesPerColor = 3;
		std::vector<glm::vec3> colors = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 0, 1)
		};

		SDL_Window* mainWindow;
		SDL_GLContext mainContext;

		UIManager uiManager;

		Camera camera;
		Brush brush;
		std::vector<Cube*> cubes;

		glm::vec3 lightPos;

		void log(char* msg, int line = -1);
		bool areVec3Equal(glm::vec3 x, glm::vec3 y);
		bool setOpenGLAttributes();

		void checkCollision();
		void checkWinState();
		void clearBackground(float r, float g, float b);
		void updateTitle();
		bool initGame();
		void reset();
	};
}

