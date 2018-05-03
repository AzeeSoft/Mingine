#include "FinalGame.h"
#include <iostream>
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace mingine;

namespace azee
{
	FinalGame* FinalGame::finalGameInstance = nullptr;

	FinalGame::FinalGame() : camera(*this), model(*this)
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

	void FinalGame::clearBackground(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	bool FinalGame::initGame()
	{
		// Initialize SDL's Video subsystem
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "Failed to init SDL\n";
			return false;
		}

		// Create our window 
		mainWindow = SDL_CreateWindow("Final Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);

		if (!mainWindow)
		{
			std::cout << "Unable to create window\n";
			log("SDLError: Error creating a window", __LINE__);
			return false;
		}

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

	void FinalGame::start()
	{
		initGame();
		model.start();
		camera.start();

		lightPos = glm::vec3(0, 1.5f, 0);
	}

	void FinalGame::update()
	{
		model.update();
		camera.update();
	}

	void FinalGame::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		clearBackground(0, 0, 0);
		if (keys[21])
		{
			clearBackground(1, 0, 0);
		}
		else if (keys[10])
		{
			clearBackground(0, 1, 0);
		}
		else if (keys[5])
		{
			clearBackground(0, 0, 1);
		}

		model.draw();
		camera.draw();

		SDL_GL_SwapWindow(mainWindow);
	}

	void FinalGame::applyCommonShaderValues(Shader& shader)
	{
		shader.use();

		int viewLoc = glGetUniformLocation(shader.getProgramId(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(FinalGame::getInstance()->getCamera().getViewMatrix()));

		glm::vec3 lightColor = glm::vec3(0.7, 0.8, 0.5);

		int lightColorLoc = glGetUniformLocation(shader.getProgramId(), "lightColor");
		glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);

		int lightPosLoc = glGetUniformLocation(shader.getProgramId(), "lightPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

		glm::vec3 cameraPos = camera.getCameraPos();

		int viewPosLoc = glGetUniformLocation(shader.getProgramId(), "viewPos");
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
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

