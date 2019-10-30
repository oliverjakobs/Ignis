#pragma once

#include "Logger.h"
#include "Timer.h"

#if _DEBUG
	
#define OBELISK_ENABLE_LOGGING
#define OBELISK_ENABLE_ASSERT
#define OBELISK_ENABLE_CHRONO

#endif

// -----------------------------------------------------------------------------------
// Logging
// -----------------------------------------------------------------------------------
#ifdef OBELISK_ENABLE_LOGGING

#define OBELISK_INFO(s, ...)		obelisk::Logger::Info(s, __VA_ARGS__)
#define OBELISK_TRACE(s, ...)		obelisk::Logger::Trace(s, __VA_ARGS__)
#define OBELISK_WARN(s, ...)		obelisk::Logger::Warn(s, __VA_ARGS__)
#define OBELISK_ERROR(s, ...)		obelisk::Logger::Error(s, __VA_ARGS__)
#define OBELISK_CRITICAL(s, ...)	obelisk::Logger::Critical(s, __VA_ARGS__)

#else

#define OBELISK_INFO(s, ...)
#define OBELISK_TRACE(s, ...)
#define OBELISK_WARN(s, ...)
#define OBELISK_ERROR(s, ...)
#define OBELISK_CRITICAL(s, ...)

#endif

// -----------------------------------------------------------------------------------
// Assert
// -----------------------------------------------------------------------------------
#ifdef OBELISK_ENABLE_ASSERT

#define OBELISK_ASSERT(x, ...) { if(!(x)) { OBELISK_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#else

#define OBELISK_ASSERT(x, ...)

#endif

// -----------------------------------------------------------------------------------
// Timer
// -----------------------------------------------------------------------------------
#ifdef OBELISK_ENABLE_CHRONO

#define OBELISK_CHRONO()				obelisk::Chrono chrono
#define OBELISK_CHRONO_GET_ELAPSED()	chrono.GetElapsedMS()
#define OBELISK_CHRONO_RESET()			chrono.Reset()

#else

#define OBELISK_CHRONO()
#define OBELISK_CHRONO_GET_ELAPSED()
#define OBELISK_CHRONO_RESET()

#endif

#if defined(OBELISK_ENABLE_LOGGING) && defined(OBELISK_ENABLE_CHRONO)

#define OBELISK_CHRONO_TRACE(s) OBELISK_TRACE(s, chrono.GetElapsedMS())

#else

#define OBELISK_CHRONO_TRACE(s)

#endif