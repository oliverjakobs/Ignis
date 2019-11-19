#pragma once

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

	struct Line
	{
		glm::vec2 Start;
		glm::vec2 End;

		Line() : Start(glm::vec2()), End(glm::vec2()) {}
		Line(const glm::vec2& s, const glm::vec2& e) : Start(s), End(e) {}
		Line(float sx, float sy, float ex, float ey) : Start(glm::vec2(sx, sy)), End(glm::vec2(ex, ey)) {}

		friend Line operator+(Line lhs, const glm::vec2& rhs) 
		{
			lhs.Start += rhs; 
			lhs.End += rhs; 
			return lhs;
		}

		friend Line operator-(Line lhs, const glm::vec2& rhs)
		{
			lhs.Start -= rhs;
			lhs.End -= rhs;
			return lhs;
		}

		operator glm::vec4() const { return { Start, End }; }
	};
}