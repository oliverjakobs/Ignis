#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ignis
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_rotation(0.0f)
	{
		SetProjection(left, right, bottom, top);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewProjection = m_projection * m_view;
	}

	void OrthographicCamera::UpdateView()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);
		transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_view = glm::inverse(transform);
		m_viewProjection = m_projection * m_view;
	}
}