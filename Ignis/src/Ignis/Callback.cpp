#include "Callback.h"

static std::function<void(ignisErrorLevel level, const std::string&)> s_errorCallback;

void ignisSetErrorCallback(std::function<void(ignisErrorLevel level, const std::string&)> callback)
{
	s_errorCallback = callback;
}

void _ignisErrorCallback(ignisErrorLevel level, const std::string& desc)
{
	s_errorCallback(level, desc);
}