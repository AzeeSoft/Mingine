#include "FinalGame.h"
#include <iostream>
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL_ttf.h>

using namespace mingine;

namespace azee
{
	FinalGame* FinalGame::finalGameInstance = nullptr;

	FinalGame::FinalGame() : uiManager(*this), camera(*this), brush(*this, glm::vec3(), glm::vec3())
	{
		finalGameInstance = this;
	}

	FinalGame::~FinalGame()
	{
		SDL_GL_DeleteContext(mainContext);
		SDL_DestroyWindow(mainWindow);
		SDL_Quit();

		delete finalGameInstance;
	}

	void FinalGame::log(char* msg, int line)
	{
		std::cout << "Line " << line << ": " << msg;
	}

	bool FinalGame::areVec3Equal(glm::vec3 x, glm::vec3 y)
	{
		return (x.x == y.x && x.y == y.y && x.z == y.z);
	}

	bool FinalGame::setOpenGLAttributes()
	{
		// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// 3.3 is part of the modern versions of OpenGL, but most video cards whould be able to run it
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		// enable double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return true;
	}

	void FinalGame::checkCollision()
	{
		for (int i = 0; i < cubes.size(); i++)
		{
			Cube* cube = cubes[i];
			cube->update();

			float dist = glm::distance(brush.position, cube->position);
			if (dist <= 0.55f)
			{
				// Collision detected
				if (cube->wireframe && areVec3Equal(cube->color, brush.color))
				{
					cube->wireframe = false;
				}
			}
		}
	}

	void FinalGame::clearBackground(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	bool FinalGame::initGame()
	{
		initPlatform(screenWidth, screenHeight, false, true);
		mainWindow = getWindow();

		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_TRUE);

		// Create our opengl context and attach it to our window
		mainContext = SDL_GL_CreateContext(mainWindow);

		setOpenGLAttributes();

		// This makes our buffer swap syncronized with the monitor's vertical refresh
		SDL_GL_SetSwapInterval(1);

		// Init GLEW
#ifndef __APPLE__
		glewExperimental = GL_TRUE;
#endif

		glewInit();

		glEnable(GL_DEPTH_TEST);

		return true;
	}

	void FinalGame::reset()
	{
		elapsedTime = 0;

		for (int i = 0; i < cubes.size(); i++)
		{
			Cube* cube = cubes[i];
			cube->color = colors[i % colors.size()];
			cube->wireframe = true;
			cube->relocate(maxBound);
		}
	}

	void FinalGame::start()
	{
		initGame();

		lightPos = glm::vec3(1, 1.5f, 0);

		int totalCubes = colors.size() * cubesPerColor;

		for (int i = 0; i < totalCubes; i++)
		{
			Cube* cube = new Cube(*this, glm::vec3(0, 0, 0), colors[i % colors.size()]);
			cube->relocate(maxBound);

			cubes.push_back(cube);

			cube->start();
		}

		camera.start();
		brush.start();

		brush.color = colors[0];


		uiManager.start();
	}

	void FinalGame::update()
	{
		elapsedTime += deltaTime;

		if(elapsedTime > resetTime)
		{
			reset();
		}

		camera.update();
		brush.update();

		if (keys[30]) // 1
		{
			brush.color = colors[0];
		}
		else if (keys[31]) // 2
		{
			brush.color = colors[1];
		}
		else if (keys[32]) // 3
		{
			brush.color = colors[2];
		}

		if (keys[44] && !prevKeys[44]) // Space Down
		{
			int targetColor = 0;
			for(int i=0; i< colors.size() - 1; i++)
			{
				if(areVec3Equal(brush.color, colors[i]))
				{
					targetColor = i + 1;
					break;
				}
			}

			brush.color = colors[targetColor];
		}

		checkCollision();

		uiManager.update();
	}

	void FinalGame::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		clearBackground(1.0, 1.0, 1.0);

		for (int i = 0; i < cubes.size(); i++)
		{
			Cube* cube = cubes[i];
			cube->draw();
		}

		camera.draw();
		brush.draw();
		uiManager.draw();

		SDL_GL_SwapWindow(mainWindow);
	}

	void FinalGame::applyCommonFixedShaderValues(Shader& shader)
	{
		shader.use();

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), screenWidth / (float)screenHeight, 0.1f, 100.0f);

		int projLoc = glGetUniformLocation(shader.getProgramId(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		int lightPosLoc = glGetUniformLocation(shader.getProgramId(), "lightPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

		glm::vec3 lightColor = glm::vec3(0.7, 0.8, 0.5);

		int lightColorLoc = glGetUniformLocation(shader.getProgramId(), "lightColor");
		glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
	}

	void FinalGame::applyCommonDynamicShaderValues(Shader& shader)
	{
		shader.use();

		int viewLoc = glGetUniformLocation(shader.getProgramId(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(FinalGame::getInstance()->getCamera().getViewMatrix()));

		glm::vec3 cameraPos = camera.getCameraPos();

		int viewPosLoc = glGetUniformLocation(shader.getProgramId(), "viewPos");
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
	}

	int FinalGame::getScreenWidth() const
	{
		return screenWidth;
	}

	int FinalGame::getScreenHeight() const
	{
		return screenHeight;
	}

	Camera FinalGame::getCamera() const
	{
		return camera;
	}

	FinalGame* FinalGame::getInstance()
	{
		if (finalGameInstance == nullptr)
			finalGameInstance = new FinalGame();

		return finalGameInstance;
	}
}

