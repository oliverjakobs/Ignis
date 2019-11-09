#pragma once

#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>

#include <memory>
#include <functional>
#include <string>

// You can #define IGNIS_ASSERT(x) before the #include to avoid using assert.h.
#ifndef IGNIS_ASSERT
#include <assert.h>
#define IGNIS_ASSERT(x) assert(x)
#endif

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
}

enum class ignisErrorLevel
{
	Warn = 0,
	Error = 1,
	Critical = 2
};

void ignisSetErrorCallback(std::function<void(ignisErrorLevel level, const std::string & desc)> callback);

void _ignisErrorCallback(ignisErrorLevel level, const std::string& desc);