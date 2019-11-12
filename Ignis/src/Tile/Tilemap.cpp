#include "Tilemap.h"

#include "Obelisk/Debugger.h"

namespace tile
{
	TileMap::TileMap(const std::vector<TileID>& tiles, int width, int height, float tileSize, const TypeMap& typeMap)
		: m_width(width), m_height(height), m_tileSize(tileSize)
	{
		m_width = static_cast<int>(std::ceilf(static_cast<float>(width) / TILE_CHUNK_SIZE));
		m_height = static_cast<int>(std::ceilf(static_cast<float>(height) / TILE_CHUNK_SIZE));

		// nested loop iterating over the width and height of the map
		for (size_t chunk_y = 0; chunk_y < m_height; chunk_y++)
		{
			for (size_t chunk_x = 0; chunk_x < m_width; chunk_x++)
			{
				Chunk chunk;
				chunk.Position = glm::vec2(chunk_x, chunk_y);

				// Loading tiles into the chunks
				for (size_t tile_y = 0; tile_y < TILE_CHUNK_SIZE; tile_y++)
				{
					for (size_t tile_x = 0; tile_x < TILE_CHUNK_SIZE; tile_x++)
					{
						Tile tile;
						tile.Position = glm::vec2(tile_x, tile_y) * m_tileSize;

						size_t index_x = (tile_x + chunk_x * TILE_CHUNK_SIZE);
						size_t index_y = (tile_y + chunk_y * TILE_CHUNK_SIZE);

						size_t index = (height - (index_y + 1)) * width + index_x;

						if (index >= tiles.size())
							break;

						OBELISK_TRACE("%d, %d", index_x, index_y);

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

	glm::ivec2 TileMap::GetTilePos(float x, float y) const
	{
		return glm::ivec2(static_cast<int>(std::floor(x / m_tileSize)), static_cast<int>(std::floor(y / m_tileSize)));
	}

	glm::ivec2 TileMap::GetTilePos(const glm::vec2& pos) const
	{
		return GetTilePos(pos.x, pos.y);
	}

	size_t TileMap::GetIndex(int x, int y) const
	{
		return (m_height - static_cast<size_t>(y) - 1) * m_width + static_cast<size_t>(x);
	}

	size_t TileMap::GetIndex(const glm::ivec2& pos) const
	{
		return GetIndex(pos.x, pos.y);
	}

	size_t TileMap::GetIndexF(float x, float y) const
	{
		return GetIndex(GetTilePos(glm::vec2(x, y)));
	}

	size_t TileMap::GetIndexF(const glm::vec2& pos) const
	{
		return GetIndex(GetTilePos(pos));
	}

	Tile& TileMap::at(size_t index)
	{
		return m_tiles.at(index);
	}

	const Tile& TileMap::at(size_t index) const
	{
		return m_tiles.at(index);
	}

	Tile& TileMap::operator[](size_t index)
	{
		return m_tiles[index];
	}

	const Tile& TileMap::operator[](size_t index) const
	{
		return m_tiles[index];
	}

	std::vector<Line> TileMap::ToEdges() const
	{
		struct EdgeData
		{
			size_t edgeID[4];
			bool edgeExist[4];
		};

		unsigned int northIndex = 0;
		unsigned int southIndex = 1;
		unsigned int eastIndex = 2;
		unsigned int westIndex = 3;

		std::vector<Line> edges;
		edges.push_back(Line(0.0f, 0.0f, m_width * m_tileSize, 0.0f));
		edges.push_back(Line(m_width * m_tileSize, 0.0f, m_width * m_tileSize, m_height * m_tileSize));
		edges.push_back(Line(m_width * m_tileSize, m_height * m_tileSize, 0.0f, m_height * m_tileSize));
		edges.push_back(Line(0.0f, m_height * m_tileSize, 0.0f, 0.0f));

		std::vector<EdgeData> edgeData(m_tiles.size());

		// iterate through all tiles from top left to bottom right
		for (int x = 1; x < m_width - 1; x++)
			for (int y = 1; y < m_height - 1; y++)
			{
				// some convenient indices
				int i = y * m_width + x;		// this
				int n = (y - 1) * m_width + x;	// northern neighbour
				int s = (y + 1) * m_width + x;	// southern neighbour
				int w = y * m_width + (x - 1);	// western neighbour
				int e = y * m_width + (x + 1);	// eastern neighbour

				// if this tile is solid, check if it needs edges
				if (m_tiles[i].Type == TileType::TILE_SOLID)
				{
					// if there is no western neighbour, it needs a western edge
					if (m_tiles[w].Type != TileType::TILE_SOLID)
					{
						// either extend it from its northern neighbour (if they have one) or start a new one
						if (edgeData[n].edgeExist[westIndex])
						{
							// northern neighbour has a western edge, so extend it downwards
							edges[edgeData[n].edgeID[westIndex]].End.y += m_tileSize;
							edgeData[i].edgeID[westIndex] = edgeData[n].edgeID[westIndex];
							edgeData[i].edgeExist[westIndex] = true;
						}
						else
						{
							// northern neighbour does not have a western edge, so create one and add it to the polygon pool
							size_t edgeID = edges.size();
							edges.push_back(Line(x * m_tileSize, y * m_tileSize, x * m_tileSize, y * m_tileSize + m_tileSize));

							// update tile information with edge information
							edgeData[i].edgeID[westIndex] = edgeID;
							edgeData[i].edgeExist[westIndex] = true;
						}
					}

					// if there is no eastern neighbour, it needs a eastern edge
					if (m_tiles[e].Type != TileType::TILE_SOLID)
					{
						// either extend it from its northern neighbour (if they have one) or start a new one
						if (edgeData[n].edgeExist[eastIndex])
						{
							// northern neighbour has a eastern edge, so extend it downwards
							edges[edgeData[n].edgeID[eastIndex]].End.y += m_tileSize;
							edgeData[i].edgeID[eastIndex] = edgeData[n].edgeID[eastIndex];
							edgeData[i].edgeExist[eastIndex] = true;
						}
						else
						{
							// northern neighbour does not have a eastern edge, so create one and add it to the polygon pool
							size_t edgeID = edges.size();
							edges.push_back(Line((x + 1) * m_tileSize, y * m_tileSize, (x + 1) * m_tileSize, y * m_tileSize + m_tileSize));

							// update tile information with edge information
							edgeData[i].edgeID[eastIndex] = edgeID;
							edgeData[i].edgeExist[eastIndex] = true;
						}
					}

					// if there is no northern neighbour, it needs a northern edge
					if (m_tiles[n].Type != TileType::TILE_SOLID)
					{
						// either extend it from its western neighbour (if they have one) or start a new one
						if (edgeData[w].edgeExist[northIndex])
						{
							// western neighbour has a northern edge, so extend it eastwards
							edges[edgeData[w].edgeID[northIndex]].End.x += m_tileSize;
							edgeData[i].edgeID[northIndex] = edgeData[w].edgeID[northIndex];
							edgeData[i].edgeExist[northIndex] = true;
						}
						else
						{
							// western neighbour does not have a northern edge, so create one and add it to the polygon pool
							size_t edgeID = edges.size();
							edges.push_back(Line(x * m_tileSize, y * m_tileSize, x * m_tileSize + m_tileSize, y * m_tileSize));

							// update tile information with edge information
							edgeData[i].edgeID[northIndex] = edgeID;
							edgeData[i].edgeExist[northIndex] = true;
						}
					}

					// if there is no southern neighbour, it needs a southern edge
					if (m_tiles[s].Type != TileType::TILE_SOLID)
					{
						// either extend it from its western neighbour (if they have one) or start a new one
						if (edgeData[w].edgeExist[southIndex])
						{
							// western neighbour has a southern edge, so extend it eastwards
							edges[edgeData[w].edgeID[southIndex]].End.x += m_tileSize;
							edgeData[i].edgeID[southIndex] = edgeData[w].edgeID[southIndex];
							edgeData[i].edgeExist[southIndex] = true;
						}
						else
						{
							// western neighbour does not have a southern edge, so create one and add it to the polygon pool
							size_t edgeID = edges.size();
							edges.push_back(Line(x * m_tileSize, (y + 1) * m_tileSize, x * m_tileSize + m_tileSize, (y + 1) * m_tileSize));

							// update tile information with edge information
							edgeData[i].edgeID[southIndex] = edgeID;
							edgeData[i].edgeExist[southIndex] = true;
						}
					}
				}
			}
		return edges;
	}
}