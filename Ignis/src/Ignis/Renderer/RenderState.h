#pragma once

#include <glad/glad.h>

#include "Ignis/Core/Types.h"

#include <map>

namespace ignis
{
	class RenderState
	{
	private:
		bool m_blend;
		uint m_blend_sfactor, m_blend_dfactor;

		bool m_depth;

	public:
		RenderState();

		void SetBlend(bool enable, uint sfactor, uint dfactor);
		void SetDepthTest(bool enable);

		void Reset();
	};
}