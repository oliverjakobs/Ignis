#pragma once

#include <glm.hpp>
#include <string>
#include <map>
#include <vector>

typedef unsigned char byte;
typedef unsigned int uint;

#include <memory>

using std::unique_ptr;
using std::make_unique;

using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;

enum RenderFlip
{
	FLIP_NONE = 0,
	FLIP_HORIZONTAL = 1,
	FLIP_VERTICAL = 2
};