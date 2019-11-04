#include "RenderState.h"


namespace ignis
{
	RenderState::RenderState()
	{
		m_blend = false;
		m_blend_sfactor = m_blend_dfactor = 0;

		m_depth = false;
	}

	void RenderState::SetBlend(bool enable, uint sfactor, uint dfactor)
	{
		m_blend = enable;
		m_blend_sfactor = sfactor;
		m_blend_dfactor = dfactor;

		if (enable)
		{
			glEnable(GL_BLEND);
			glBlendFunc(sfactor, dfactor);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void RenderState::SetDepthTest(bool enable)
	{
		m_depth = enable;

		if (enable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void RenderState::Reset()
	{
		SetBlend(m_blend, m_blend_sfactor, m_blend_dfactor);
		SetDepthTest(m_depth);
	}
}