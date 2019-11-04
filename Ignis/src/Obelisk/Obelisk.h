#pragma once

#include <functional>
#include <iterator>

#include "Debugger.h"
#include "FileUtility.h"

#include "Range.h"

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

#define SAFE_DELETE(ptr) if( (ptr) != nullptr ) delete (ptr); (ptr) = nullptr;
