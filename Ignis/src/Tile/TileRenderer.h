#pragma once

#include "Tilemap.h"

#include "Ignis/Core/Buffer.h"
#include "Ignis/Core/Shader.h"
#include "Ignis/Advanced/TextureAtlas.h"

class TileRenderer
{
private:
	ignis::VertexArray m_vertexArray;
	std::shared_ptr<ignis::ArrayBuffer> m_bufferOffsets;
	std::shared_ptr<ignis::ArrayBuffer> m_bufferFrames;

	std::shared_ptr<ignis::Shader> m_shader;

	size_t m_instanceCount;
public:
	TileRenderer(size_t tiles);

	void LoadMap(const std::vector<Tile>& tiles, int rows, int columns, float tileSize);

	void RenderMap(const glm::vec3& offset, const glm::mat4& viewProjection, const std::shared_ptr<ignis::Texture>& texture);
};