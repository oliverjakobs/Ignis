#pragma once

#include "Logger.h"
#include "Timer.h"

// -----------------------------------------------------------------------------------
// Logging
// -----------------------------------------------------------------------------------
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

#endif


// -----------------------------------------------------------------------------------
// Timer
// -----------------------------------------------------------------------------------
#ifdef _DEBUG

#define DEBUG_TIMER()				Timer timer
#define DEBUG_TIMER_GET_ELAPSED()	timer.GetElapsedMS()
#define DEBUG_TIMER_RESET()			timer.Reset()

#define DEBUG_TIMER_TRACE(s)		DEBUG_TRACE(s, timer.GetElapsedMS())

#else

#define DEBUG_TIMER()
#define DEBUG_TIMER_GET_ELAPSED()
#define DEBUG_TIMER_RESET()

#define DEBUG_TIMER_TRACE(s)

#endif