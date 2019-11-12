#include "TileRenderer.h"

#include "glm/gtc/matrix_transform.hpp"

namespace tile
{
	TileRenderer::TileRenderer(size_t tiles)
	{
		m_instanceCount = tiles;

		float vertices[] =
		{
			// positions	// texcoords
			0.0f, 0.0f,		0.0f, 0.0f,
			1.0f, 0.0f,		1.0f, 0.0f,
			1.0f, 1.0f,		1.0f, 1.0f,
			0.0f, 1.0f,		0.0f, 1.0f
		};

		m_shader = std::make_shared<ignis::Shader>("res/shaders/tile.vert", "res/shaders/tile.frag");

		m_vertexArray.AddArrayBuffer(std::make_shared<ignis::ArrayBuffer>(sizeof(vertices), vertices, GL_STATIC_DRAW),
			{
				{GL_FLOAT, 2},
				{GL_FLOAT, 2}
			});

		// also set instance data
		m_bufferOffsets = std::make_shared<ignis::ArrayBuffer>(sizeof(glm::vec2) * tiles, nullptr, GL_DYNAMIC_DRAW);
		m_bufferOffsets->VertexAttribPointer(2, 2, GL_FALSE, 2, 0);
		m_bufferOffsets->VertexAttribDivisor(2, 1);
		m_vertexArray.AddArrayBuffer(m_bufferOffsets);

		m_bufferFrames = std::make_shared<ignis::ArrayBuffer>(sizeof(GLuint) * tiles, nullptr, GL_DYNAMIC_DRAW);
		m_bufferFrames->VertexAttribIPointer(3, 1, 1, 0);
		m_bufferFrames->VertexAttribDivisor(3, 1);
		m_vertexArray.AddArrayBuffer(m_bufferFrames);

		// element buffer
		m_vertexArray.LoadElementBuffer({ 0, 1, 2, 2, 3, 0 }, GL_STATIC_DRAW);
	}

	void TileRenderer::LoadMap(const std::vector<Tile>& tiles, int rows, int columns, float tileSize)
	{
		std::vector<glm::vec2> offsets;
		std::vector<GLuint> frames;

		for (auto& tile : tiles)
		{
			offsets.push_back(tile.Position);
			frames.push_back(tile.ID);
		}

		m_shader->Use();
		m_shader->SetUniform1i("u_Texture", 0);
		m_shader->SetUniform1i("u_Rows", rows);
		m_shader->SetUniform1i("u_Columns", columns);
		m_shader->SetUniform1f("u_TileSize", tileSize);

		m_bufferOffsets->BufferSubData(0, sizeof(glm::vec2) * m_instanceCount, &offsets[0]);
		m_bufferFrames->BufferSubData(0, sizeof(GLuint) * m_instanceCount, &frames[0]);
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