#pragma once

#include "Tilemap.h"

#include <algorithm>

namespace tile
{
	// convert std::vector<Line> to std::vector<glm::vec4>:
	// std::vector<glm::vec4> v(lines.begin(), lines.end());

	std::vector<glm::vec3> Visibility(const glm::vec2& pos, std::vector<Line> edges);

	// returns the type of the tile at index and TileType::TILE_EMPTY if index is out of bounds
	TileType GetTileType(const std::vector<Tile>& tiles, int32_t index);

	std::vector<Line> GetEdges(const TileMap& map);
	std::vector<Line> GetEdges(const std::vector<Tile>& tiles, size_t width, size_t height, float tileSize, const glm::vec2& offset = glm::vec2());
}