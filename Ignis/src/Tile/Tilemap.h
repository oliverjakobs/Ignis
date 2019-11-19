#pragma once

#include "Tile.h"

namespace tile
{
	class TileMap
	{
	private:
		// map settings
		int m_width;
		int m_height;

		float m_tileSize;

		std::vector<Tile> m_tiles;

	public:
		TileMap(const std::vector<TileID>& tiles, int width, int height, float tileSize, size_t chunksize, const TypeMap& typeMap);
		~TileMap();

		const int GetWidth() const { return m_width; }
		const int GetHeight() const { return m_height; }
		const glm::vec2 GetDimension() const { return glm::vec2(m_width, m_height); }

		const float GetTileSize() const { return m_tileSize; }

		std::vector<Tile> GetTiles() const { return m_tiles; }

		// get tile index
		size_t GetIndex(const glm::vec2& pos) const;

		// access chunk from an index
		Tile& at(size_t index) { return m_tiles.at(index); }
		const Tile& at(size_t index) const { return m_tiles.at(index); }
		Tile& operator[](size_t index) { return m_tiles[index]; }
		const Tile& operator[](size_t index) const { return m_tiles[index]; }
	};
}
