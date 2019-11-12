#pragma once

#include "TileConfig.h"

#include <map>
#include <vector>

#include <glm/glm.hpp>

namespace tile
{
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

	typedef uint32_t TileID;
	typedef std::map<TileID, TileType> TypeMap;

	struct Tile
	{
		TileID ID;
		glm::vec2 Position;
		TileType Type;
	};
}