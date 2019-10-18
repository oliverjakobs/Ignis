#pragma once

#include <glm/glm.hpp>

namespace ignis
{
	class Camera
	{
	protected:
		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::mat4 m_viewProjection;

		glm::vec3 m_position;

	public:
		Camera() : m_view(1.0f), m_projection(1.0f), m_viewProjection(1.0f), m_position() {}

		void SetPosition(const glm::vec3& position) { m_position = position; UpdateView(); }
		const glm::vec3& GetPosition() const { return m_position; }

		const glm::mat4& GetViewx() const { return m_view; }
		const glm::mat4& GetProjection() const { return m_projection; }
		const glm::mat4& GetViewProjection() const { return m_viewProjection; }

	protected:
		virtual void UpdateView() = 0;
	};
}