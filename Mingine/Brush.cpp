#include "Brush.h"
#include "FinalGame.h"


namespace azee
{
	Brush::Brush(NativeGameLoop& native_game_loop, const glm::vec3& pos, const glm::vec3& col)
		: Cube(native_game_loop, pos, col)
	{
		scale = glm::vec3(0.05, 0.05, 0.05);
		wireframe = false;
	}

	Brush::~Brush()
	{
	}

	void Brush::update()
	{
		Cube::update();

		Camera camera = FinalGame::getInstance()->getCamera();
		position = camera.cameraPos;

		glm::vec3 normalFront = glm::normalize(camera.cameraFront);
		normalFront *= 1.5;

		position += normalFront;
	}
}

