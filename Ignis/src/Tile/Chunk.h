#pragma once

#include "Tile.h"

namespace tile
{
	struct Chunk
	{
		size_t Index;
		glm::vec2 Position;
		std::vector<Tile> Tiles;

		// access tile from an index
		Tile& at(size_t index) { return Tiles.at(index); }
		const Tile& at(size_t index) const { return Tiles.at(index); }
		Tile& operator[](size_t index) { return Tiles[index]; }
		const Tile& operator[](size_t index) const { return Tiles[index]; }
	};
}