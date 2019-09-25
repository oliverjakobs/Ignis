#pragma once

#include <spdlog/spdlog.h>
#include "Timer.h"

enum class LogLevel
{
	Trace = SPDLOG_LEVEL_TRACE,
	Debug = SPDLOG_LEVEL_DEBUG,
	Info = SPDLOG_LEVEL_INFO,
	Warn = SPDLOG_LEVEL_WARN,
	Error = SPDLOG_LEVEL_ERROR
};

// wrapper for spdlog
struct Logger
{
	inline static void SetLevel(LogLevel lvl)
	{
		spdlog::set_level((spdlog::level::level_enum)lvl);
	}

	inline static void SetFormat(const std::string& pattern)
	{
		spdlog::set_pattern(pattern);
	}

	template<typename... Args>
	inline static void Info(const char* fmt, const Args& ... args)
	{
		spdlog::info(fmt, args...);
	}

	template<typename... Args>
	inline static void Trace(const char* fmt, const Args& ... args)
	{
		spdlog::trace(fmt, args...);
	}

	template<typename... Args>
	inline static void Warn(const char* fmt, const Args& ... args)
	{
		spdlog::warn(fmt, args...);
	}

	template<typename... Args>
	inline static void Error(const char* fmt, const Args& ... args)
	{
		spdlog::error(fmt, args...);
	}

	template<typename... Args>
	inline static void Critical(const char* fmt, const Args& ... args)
	{
		spdlog::critical(fmt, args...);
	}
};

#ifdef _DEBUG

#define DEBUG_INFO(s, ...) Logger::Info(s, __VA_ARGS__)
#define DEBUG_TRACE(s, ...) Logger::Trace(s, __VA_ARGS__)
#define DEBUG_WARN(s, ...) Logger::Warn(s, __VA_ARGS__)
#define DEBUG_ERROR(s, ...) Logger::Error(s, __VA_ARGS__)
#define DEBUG_CRITICAL(s, ...) Logger::Critical(s, __VA_ARGS__)

#define DEBUG_ASSERT(x, ...) { if(!(x)) { DEBUG_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#else

#define DEBUG_INFO(s, ...)
#define DEBUG_TRACE(s, ...)
#define DEBUG_WARN(s, ...)
#define DEBUG_ERROR(s, ...)
#define DEBUG_CRITICAL(s, ...)

#define DEBUG_ASSERT(x, ...)

#endif // DEBUG