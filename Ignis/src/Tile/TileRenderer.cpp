#include "TileRenderer.h"

#include "glm/gtc/matrix_transform.hpp"

namespace tile
{
	TileRenderer::TileRenderer(size_t chunks, int rows, int columns, float tileSize)
		: m_tileSize(tileSize)
	{
		m_instanceCount = chunks * TILE_CHUNK_SIZE * TILE_CHUNK_SIZE;

		float vertices[] =
		{
			// positions	// texcoords
			0.0f, 0.0f,		0.0f, 0.0f,
			1.0f, 0.0f,		1.0f, 0.0f,
			1.0f, 1.0f,		1.0f, 1.0f,
			0.0f, 1.0f,		0.0f, 1.0f
		};

		m_shader = std::make_shared<ignis::Shader>("res/shaders/tile.vert", "res/shaders/tile.frag");
		m_shader->Use();
		m_shader->SetUniform1i("u_Texture", 0);
		m_shader->SetUniform1i("u_Rows", rows);
		m_shader->SetUniform1i("u_Columns", columns);
		m_shader->SetUniform1f("u_TileSize", tileSize);

		m_vertexArray.AddArrayBuffer(std::make_shared<ignis::ArrayBuffer>(sizeof(vertices), vertices, GL_STATIC_DRAW),
			{
				{GL_FLOAT, 2},
				{GL_FLOAT, 2}
			});

		// also set instance data
		m_bufferOffsets = std::make_shared<ignis::ArrayBuffer>(sizeof(glm::vec2) * chunks * TILE_CHUNK_SIZE * TILE_CHUNK_SIZE, nullptr, GL_DYNAMIC_DRAW);
		m_bufferOffsets->VertexAttribPointer(2, 2, GL_FALSE, 2, 0);
		m_bufferOffsets->VertexAttribDivisor(2, 1);
		m_vertexArray.AddArrayBuffer(m_bufferOffsets);

		m_bufferFrames = std::make_shared<ignis::ArrayBuffer>(sizeof(GLuint) * chunks * TILE_CHUNK_SIZE * TILE_CHUNK_SIZE, nullptr, GL_DYNAMIC_DRAW);
		m_bufferFrames->VertexAttribIPointer(3, 1, 1, 0);
		m_bufferFrames->VertexAttribDivisor(3, 1);
		m_vertexArray.AddArrayBuffer(m_bufferFrames);

		// element buffer
		m_vertexArray.LoadElementBuffer({ 0, 1, 2, 2, 3, 0 }, GL_STATIC_DRAW);
	}

	void TileRenderer::LoadChunk(const Chunk& chunk, int index)
	{
		std::vector<glm::vec2> offsets;
		std::vector<GLuint> frames;

		glm::vec2 offset = chunk.GetOffset(m_tileSize);

		for (auto& tile : chunk.Tiles)
		{
			offsets.push_back(tile.Position + offset);
			frames.push_back(tile.ID);
		}

		int bufferOffset = index * (TILE_CHUNK_SIZE * TILE_CHUNK_SIZE);

		m_bufferOffsets->BufferSubData(sizeof(glm::vec2) * bufferOffset, sizeof(glm::vec2) * offsets.size(), &offsets[0]);
		m_bufferFrames->BufferSubData(sizeof(GLuint) * bufferOffset, sizeof(GLuint) * frames.size(), &frames[0]);
	}

	void TileRenderer::RenderMap(const glm::vec3& offset, const glm::mat4& viewProjection, const std::shared_ptr<ignis::Texture>& texture)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), offset);

		m_shader->Use();

		m_shader->SetUniformMat4("u_ViewProjection", viewProjection);
		m_shader->SetUniformMat4("u_Model", model);

		m_vertexArray.Bind();

		texture->Bind();
		glDrawElementsInstanced(GL_TRIANGLES, m_vertexArray.GetElementCount(), GL_UNSIGNED_INT, nullptr, m_instanceCount);
	}
}