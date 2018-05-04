#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameObject.h"
#include <string>
#include "Shader.h"

using namespace mingine;

namespace azee
{
	class Cube : GameObject
	{
	public:
		bool wireframe = false;

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 color;

		Cube(NativeGameLoop& native_game_loop, glm::vec3 pos, glm::vec3 col);
		~Cube();

		void relocate(float maxBound);

		void start() override;
		void update() override;
		void draw() override;

	protected:

	private:
		Shader shader;

		unsigned int VAO, VBO, EBO;

		void prepareGeometry();
		void prepareShader();
	};
}

