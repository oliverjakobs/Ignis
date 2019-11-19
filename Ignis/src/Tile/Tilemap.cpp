#include "Tilemap.h"

namespace tile
{
	TileMap::TileMap(const std::vector<TileID>& tiles, int width, int height, float tileSize, size_t chunksize, const TypeMap& typeMap)
		: m_width(width), m_height(height), m_tileSize(tileSize)
	{
		// nested loop iterating over the width and height of the map
		for (size_t i = 0; i < m_height; i++)
		{
			for (size_t j = 0; j < m_width; j++)
			{
				Tile tile;
				tile.Position = glm::vec2(j, m_height - (i + 1)) * m_tileSize;
				tile.ID = tiles.at(i * m_width + j);

				// Get the type for the id;
				if (typeMap.find(tile.ID) != typeMap.end())
					tile.Type = typeMap.at(tile.ID);
				else
					tile.Type = TileType::TILE_EMPTY;

				m_tiles.push_back(tile);
			}
		}
	}

	TileMap::~TileMap()
	{

	}

	size_t TileMap::GetIndex(const glm::vec2& pos) const
	{
		size_t x = static_cast<size_t>(std::floorf(pos.x / m_tileSize));
		size_t y = static_cast<size_t>(std::floorf(pos.y / m_tileSize));

		return y * static_cast<int>(std::ceilf(static_cast<float>(m_width))) + x;
	}
}