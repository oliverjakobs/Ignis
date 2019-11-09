#pragma once

#include <glad/glad.h>

#include "Ignis/Core/Api.h"

namespace ignis
{
	class RenderState
	{
	private:
		bool m_blend;
		GLenum m_blend_sfactor, m_blend_dfactor;

		bool m_depth;

		bool m_cull;

	public:
		RenderState();

		void SetBlend(bool enable, GLenum sfactor, GLenum dfactor);
		void SetDepthTest(bool enable);
		void SetCullFace(bool enable);

		void Reset();
	};
}