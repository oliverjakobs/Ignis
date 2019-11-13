#pragma once

#include "Tilemap.h"
#include "Algorithm.h"

#include "Ignis/Core/Buffer.h"
#include "Ignis/Core/Shader.h"
#include "Ignis/Core/Texture.h"

namespace tile
{
	class TileRenderer
	{
	private:
		ignis::VertexArray m_vertexArray;
		std::shared_ptr<ignis::ArrayBuffer> m_bufferOffsets;
		std::shared_ptr<ignis::ArrayBuffer> m_bufferFrames;
		std::shared_ptr<ignis::Texture> m_texture;

		std::shared_ptr<ignis::Shader> m_shader;

		size_t m_tileCount;
	public:
		TileRenderer(const TileMap& map, const std::shared_ptr<ignis::Texture>& texture, size_t rows, size_t columns);

		void LoadChunk(const Chunk& chunk, const TileMap& map);

		void RenderMap(const glm::vec3& offset, const glm::mat4& viewProjection);
	};
}