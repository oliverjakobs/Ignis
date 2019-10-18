#pragma once

#include "Camera.h"

namespace ignis
{
	class OrthographicCamera : public Camera
	{
	private:
		float m_rotation;

	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		void SetRotation(float rotation) { m_rotation = rotation; UpdateView(); }
		const float GetRotation() const { return m_rotation; }

	private:
		void UpdateView() override;
	};
}
