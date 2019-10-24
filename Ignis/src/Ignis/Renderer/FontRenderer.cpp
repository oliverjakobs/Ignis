#include "FontRenderer.h"

namespace ignis
{
	void FontRenderer::RenderText(Font& font, const std::string& text, float x, float y)
	{
		font.Bind();

		for (auto& c : text)
		{
			if (font.LoadCharQuad(c, &x, &y))
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Render quad
		}
	}
}
