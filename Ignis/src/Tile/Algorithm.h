#pragma once

#include "Tilemap.h"

#include <algorithm>

namespace tile
{
	std::vector<glm::vec3> Visibility(const glm::vec2& pos, std::vector<Line> edges);

	std::vector<Tile> GetTiles(const TileMap& map);
	std::vector<Line> GetEdges(const std::vector<Tile>& tiles, int width, int height, float tileSize);
}