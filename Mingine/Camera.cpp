#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


namespace azee
{
	Camera::Camera(mingine::NativeGameLoop& native_game_loop): GameObject(native_game_loop)
	{
		yaw = -90;
		pitch = 0;

		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		viewMatrix = glm::lookAt(cameraPos,
		                         cameraPos + cameraFront,
		                         cameraUp);
	}


	void Camera::start()
	{

	}

	void Camera::update()
	{

		float xoffset = mingine::deltaMouseX;
		float yoffset = -mingine::deltaMouseY;

		float sensitivity = 0.13f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);

		float cameraSpeed = 2.5f * mingine::deltaTime;
		if (mingine::keys[26])
		{
			cameraPos += cameraSpeed * cameraFront;
		}
		else if (mingine::keys[22])
		{
			cameraPos -= cameraSpeed * cameraFront;
		}

		if (mingine::keys[4])
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		else if (mingine::keys[7])
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		viewMatrix = glm::lookAt(cameraPos,
		                         cameraPos + cameraFront,
		                         cameraUp);
	}

	void Camera::draw()
	{
	}

	glm::vec3 Camera::getCameraPos() const
	{
		return cameraPos;
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return viewMatrix;
	}

	Camera::~Camera()
	{
	}
}

