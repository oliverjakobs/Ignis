#include "Camera.h"

Camera::Camera()
	: Position(0.0f, 0.0f, 0.0f), WorldUp(0.0f, 1.0f, 0.0f), Front(0.0f, 0.0f, -1.0f), Yaw(-90.0f), Pitch(0.0f)
{
	UpdateVectors();
}

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
	: Position(position), WorldUp(up), Front(0.0f, 0.0f, -1.0f), Yaw(yaw), Pitch(pitch)
{
	UpdateVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Position(posX, posY, posZ), WorldUp(upX, upY, upZ), Front(0.0f, 0.0f, -1.0f), Yaw(yaw), Pitch(pitch)
{
	UpdateVectors();
}

glm::mat4 Camera::View() const
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Move(const glm::vec3& move)
{
	Position += move;
}

void Camera::YawPitch(float yaw, float pitch, bool constrainPitch)
{
	Yaw += yaw;
	Pitch += pitch;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	UpdateVectors();
}

void Camera::UpdateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
