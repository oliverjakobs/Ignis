#pragma once

#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>

#include <memory>

namespace ignis
{
	typedef glm::vec4 color;

	const color WHITE	= { 1.0f, 1.0f, 1.0f, 1.0f };
	const color BLACK	= { 0.0f, 0.0f, 0.0f, 1.0f };
	const color RED		= { 1.0f, 0.0f, 0.0f, 1.0f };
	const color GREEN	= { 0.0f, 1.0f, 0.0f, 1.0f };
	const color BLUE	= { 0.0f, 0.0f, 1.0f, 1.0f };
	const color CYAN	= { 0.0f, 1.0f, 1.0f, 1.0f };
	const color MAGENTA = { 1.0f, 0.0f, 1.0f, 1.0f };
	const color YELLOW	= { 1.0f, 1.0f, 0.0f, 1.0f };

	typedef unsigned char byte;
	typedef unsigned int uint;
}
