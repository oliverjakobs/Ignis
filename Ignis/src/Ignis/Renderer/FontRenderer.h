#pragma once

#include "Font.h"

#include "Core/Shader.h"

namespace ignis
{
	struct FontRenderer
	{
		static void RenderText(Font& font, const std::string& text, float x, float y);
	};
}
