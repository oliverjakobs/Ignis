#pragma once

#include "TileConfig.h"

#include "Ignis/Core/Api.h"

enum class TileType
{
	TILE_EMPTY = 0,
	TILE_SOLID = 1,
	TILE_SLOPE_LEFT = 2,
	TILE_SLOPE_RIGHT = 3,
	TILE_PLATFORM = 4,
	// not implemented yet
	TILE_CEIL_LEFT = 5,
	TILE_CEIL_RIGHT = 6
};

struct Tile
{
	unsigned int ID;
	glm::vec2 Position;
	TileType Type;
};


enum class BodyType
{
	BODY_STATIC,		// does not move by any means 
	BODY_DYNAMIC		// can be moved, affected by forces
};

struct BodyDef
{
	float HalfWidth;
	float HalfHeight;
	BodyType Type;
};

BodyType FromString(const std::string& str);