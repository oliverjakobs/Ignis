#pragma once

#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>

typedef glm::vec4 Color;

typedef unsigned char byte;
typedef unsigned int uint;


typedef std::vector<glm::vec2> Vertices;
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