#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameObject.h"
#include "Shader.h"
#include "Cube.h"

namespace azee
{
	class Brush : public Cube
	{
	public:
		Brush(NativeGameLoop& native_game_loop, const glm::vec3& pos, const glm::vec3& col);
		~Brush();

		void update() override;
	};
}

