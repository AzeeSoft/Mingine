#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"

namespace azee
{
	class Camera : mingine::GameObject
	{
	public:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		Camera(mingine::NativeGameLoop& native_game_loop);
		~Camera();

		void start() override;
		void update() override;
		void draw() override;

		glm::vec3 getCameraPos() const;
		glm::mat4 getViewMatrix() const;

	private:
		float yaw, pitch;


		glm::mat4 viewMatrix;
	};
}

