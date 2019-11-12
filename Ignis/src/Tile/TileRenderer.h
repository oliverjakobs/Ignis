#pragma once

#include "Tilemap.h"

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

		std::shared_ptr<ignis::Shader> m_shader;

		size_t m_instanceCount;
		float m_tileSize;
	public:
		TileRenderer(size_t chunks, int rows, int columns, float tileSize);

		void LoadChunk(const Chunk& chunk, int index);

		void RenderMap(const glm::vec3& offset, const glm::mat4& viewProjection, const std::shared_ptr<ignis::Texture>& texture);
	};
}