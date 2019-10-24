#pragma once

#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>

#include <memory>

namespace ignis
{
	typedef glm::vec4 Color;

	const Color WHITE	= { 1.0f, 1.0f, 1.0f, 1.0f };
	const Color BLACK	= { 0.0f, 0.0f, 0.0f, 1.0f };
	const Color RED		= { 1.0f, 0.0f, 0.0f, 1.0f };
	const Color GREEN	= { 0.0f, 1.0f, 0.0f, 1.0f };
	const Color BLUE	= { 0.0f, 0.0f, 1.0f, 1.0f };
	const Color CYAN	= { 0.0f, 1.0f, 1.0f, 1.0f };
	const Color MAGENTA = { 1.0f, 0.0f, 1.0f, 1.0f };
	const Color YELLOW	= { 1.0f, 1.0f, 0.0f, 1.0f };

	typedef unsigned char byte;
	typedef unsigned int uint;
}
