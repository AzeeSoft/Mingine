#pragma once
#include "GameObject.h"
#include <string>
#include "Shader.h"

using namespace mingine;

namespace azee
{
	class Model : GameObject
	{
	public:
		Model(NativeGameLoop& native_game_loop);
		~Model();

		void start() override;
		void update() override;
		void draw() override;

	private:
		Shader shader;

		unsigned int VAO, VBO, EBO;
		float elapsedTime;

		void prepareGeometry();
		void prepareShader();
	};
}

