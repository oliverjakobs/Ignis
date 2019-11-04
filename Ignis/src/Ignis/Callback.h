#pragma once

#include <functional>
#include <string>

enum class ignisErrorLevel
{
	Warn = 0,
	Error = 1,
	Critical = 2
};

void ignisSetErrorCallback(std::function<void(ignisErrorLevel level, const std::string& desc)> callback);

void _ignisErrorCallback(ignisErrorLevel level, const std::string& desc);