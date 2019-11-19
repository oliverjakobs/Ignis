#pragma once

#include "Chunk.h"

namespace tile
{
	class TileMap
	{
	private:
		// map settings
		int m_width;
		int m_height;

		float m_tileSize;
		size_t m_chunkSize;

		size_t m_chunkCountX;
		size_t m_chunkCountY;

		std::vector<Chunk> m_chunks;

	public:
		TileMap(const std::vector<TileID>& tiles, int width, int height, float tileSize, size_t chunksize, const TypeMap& typeMap);
		~TileMap();

		const int GetWidth() const { return m_width; }
		const int GetHeight() const { return m_height; }
		const glm::vec2 GetDimension() const { return glm::vec2(m_width, m_height); }

		const float GetTileSize() const { return m_tileSize; }
		const size_t GetChunkSize() const { return m_chunkSize; }

		const size_t GetChunkCountX() const { return m_chunkCountX; }
		const size_t GetChunkCountY() const { return m_chunkCountY; }

		std::vector<Chunk> GetChunks() const { return m_chunks; }

		glm::vec2 GetChunkOffset(const Chunk& chunk) const;

		// get chunk index
		size_t GetIndex(const glm::vec2& pos) const;

		// access chunk from an index
		Chunk& at(size_t index) { return m_chunks.at(index); }
		const Chunk& at(size_t index) const { return m_chunks.at(index); }
		Chunk& operator[](size_t index) { return m_chunks[index]; }
		const Chunk& operator[](size_t index) const { return m_chunks[index]; }
	};
}
