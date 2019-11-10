#include "TileRenderer.h"

TileRenderer::TileRenderer(const std::vector<Tile>& tiles, int rows, int columns, float size)
{
	std::vector<glm::vec2> translations;
	std::vector<GLuint> frames;

	for (auto& tile : tiles)
	{
		translations.push_back(tile.Position);
		frames.push_back(tile.ID);
	}

	m_instanceCount = tiles.size();

	float vertices[] =
	{
		// positions	// texcoords
		0.0f, 0.0f,		0.0f, 0.0f,
		size, 0.0f,		1.0f, 0.0f,
		size, size,		1.0f, 1.0f,
		0.0f, size,		0.0f, 1.0f
	};

	m_shader = std::make_shared<ignis::Shader>("res/shaders/tile.vert", "res/shaders/tile.frag");
	m_shader->Use();
	m_shader->SetUniform1i("u_Texture", 0);
	m_shader->SetUniform1i("u_Rows", rows);
	m_shader->SetUniform1i("u_Columns", columns);

	m_vertexArray.AddArrayBuffer(std::make_shared<ignis::ArrayBuffer>(sizeof(vertices), vertices, GL_STATIC_DRAW),
	{
		{GL_FLOAT, 2},
		{GL_FLOAT, 2}
	});

	// also set instance data
	auto t_vbo = std::make_shared<ignis::ArrayBuffer>(sizeof(glm::vec2) * translations.size(), &translations[0], GL_STATIC_DRAW);
	t_vbo->VertexAttribPointer(2, 2, GL_FALSE, 2, 0);
	t_vbo->VertexAttribDivisor(2, 1);
	m_vertexArray.AddArrayBuffer(t_vbo);

	auto f_vbo = std::make_shared<ignis::ArrayBuffer>(sizeof(GLuint) * frames.size(), &frames[0], GL_STATIC_DRAW);
	f_vbo->VertexAttribIPointer(3, 1, 1, 0);
	f_vbo->VertexAttribDivisor(3, 1);
	m_vertexArray.AddArrayBuffer(f_vbo);

	// element buffer
	m_vertexArray.LoadElementBuffer({ 0, 1, 2, 2, 3, 0 }, GL_STATIC_DRAW);
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
