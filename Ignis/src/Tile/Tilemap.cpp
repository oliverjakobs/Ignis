#include "Tilemap.h"

namespace tile
{
	TileMap::TileMap(const std::vector<TileID>& tiles, int width, int height, float tileSize, size_t chunksize, const TypeMap& typeMap)
		: m_width(width), m_height(height), m_tileSize(tileSize), m_chunkSize(chunksize)
	{
		m_chunkCountX = static_cast<int>(std::ceilf(static_cast<float>(width) / m_chunkSize));
		m_chunkCountY = static_cast<int>(std::ceilf(static_cast<float>(height) / m_chunkSize));

		// nested loop iterating over the width and height of the map
		for (size_t chunk_y = 0; chunk_y < m_chunkCountY; chunk_y++)
		{
			for (size_t chunk_x = 0; chunk_x < m_chunkCountX; chunk_x++)
			{
				Chunk chunk;
				chunk.Index = chunk_y * m_chunkCountX + chunk_x;
				chunk.Position = glm::vec2(chunk_x, chunk_y);

				// Loading tiles into the chunks
				for (size_t tile_y = 0; tile_y < m_chunkSize; tile_y++)
				{
					for (size_t tile_x = 0; tile_x < m_chunkSize; tile_x++)
					{
						Tile tile;
						tile.Position = glm::vec2(tile_x, tile_y) * m_tileSize;

						size_t index_x = (tile_x + chunk_x * m_chunkSize);
						size_t index_y = (tile_y + chunk_y * m_chunkSize);

						size_t index = (height - (index_y + 1)) * width + index_x;

						if (index >= tiles.size())
							break;

						tile.ID = tiles.at(index);
						
						// Get the type for the id;
						if (typeMap.find(tile.ID) != typeMap.end())
							tile.Type = typeMap.at(tile.ID);
						else
							tile.Type = TileType::TILE_EMPTY;

						chunk.Tiles.push_back(tile);
					}
				}

				m_chunks.push_back(chunk);
			}
		}
	}

	TileMap::~TileMap()
	{

	}

	glm::vec2 TileMap::GetChunkOffset(const Chunk& chunk) const
	{
		return chunk.Position * static_cast<float>(m_chunkSize)* m_tileSize;
	}

	size_t TileMap::GetIndex(const glm::vec2& pos) const
	{
		size_t x = static_cast<size_t>(std::floorf(pos.x / (m_chunkSize * m_tileSize)));
		size_t y = static_cast<size_t>(std::floorf(pos.y / (m_chunkSize * m_tileSize)));

		return y * static_cast<int>(std::ceilf(static_cast<float>(m_width) / m_chunkSize)) + x;
	}
}