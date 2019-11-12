#pragma once

#include "Tile.h"
#include "Math.h"

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
		TileMap(const std::vector<TileID>& tiles, int width, int height, float tileSize, const TypeMap& typeMap);
		~TileMap();

		const int GetWidth() const { return m_width; }
		const int GetHeight() const { return m_height; }
		const float GetTileSize() const { return m_tileSize; }
		const glm::vec2 GetDimension() const { return glm::vec2(m_width, m_height); }

		// calculate map coords from world coords
		glm::ivec2 GetTilePos(float x, float y) const;
		glm::ivec2 GetTilePos(const glm::vec2& pos) const;

		// get index from map coords
		size_t GetIndex(int x, int y) const;
		size_t GetIndex(const glm::ivec2& pos) const;
		// get index from world coords
		size_t GetIndexF(float x, float y) const;
		size_t GetIndexF(const glm::vec2& pos) const;

		// access tile from an index
		Tile& at(size_t index);
		const Tile& at(size_t index) const;
		Tile& operator[](size_t index);
		const Tile& operator[](size_t index) const;

		std::vector<Tile> GetTiles() const;
		std::vector<Line> ToEdges() const;
	};
}
