#pragma once

#include <glad/glad.h>

#include "Core/Types.h"

namespace ignis
{
	enum class RenderState
	{
		None			= 0 << 0,
		EnableBlending	= 1 << 0,
		EnableDepthTest = 1 << 1
	};

	struct RenderSettings
	{
		RenderState State;
		std::pair<uint, uint> BlendFunc;
	};
}