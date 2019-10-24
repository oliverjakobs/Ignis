#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ignis
{
	struct FpsCamera
	{
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;

		FpsCamera();
		FpsCamera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch);
		FpsCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		glm::mat4 View() const;

		void Move(const glm::vec3& move);
		void YawPitch(float yaw, float pitch, bool constrainPitch = true);

		void UpdateVectors();
	};
}