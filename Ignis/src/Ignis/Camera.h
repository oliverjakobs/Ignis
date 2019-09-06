#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Types.h"

namespace ignis
{
	struct Camera
	{
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;

		Camera();
		Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch);
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		glm::mat4 View() const;

		void Move(const glm::vec3& move);
		void YawPitch(float yaw, float pitch, bool constrainPitch = true);

		void UpdateVectors();
	};
}