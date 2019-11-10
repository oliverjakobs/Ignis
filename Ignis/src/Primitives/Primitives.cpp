#include "Primitives.h"

static const int MAX_VERTICES = 2 * 1024;

using namespace ignis;

struct RendererStorage
{
	std::shared_ptr<VertexArray> VertexArray;
	std::shared_ptr<Shader> Shader;

	std::vector<glm::vec2> Vertices;
};

static RendererStorage* s_renderData;

void Primitives::Init()
{
	s_renderData = new RendererStorage();

	s_renderData->VertexArray = std::make_shared<VertexArray>();

	s_renderData->VertexArray->AddArrayBuffer(std::make_shared<ArrayBuffer>(sizeof(glm::vec2) * MAX_VERTICES, nullptr, GL_DYNAMIC_DRAW),
		{
			{GL_FLOAT, 2}
		});

	s_renderData->Shader = std::make_shared<Shader>("res/shaders/lines.vert", "res/shaders/lines.frag");
}

void Primitives::Destroy()
{
	delete s_renderData;
}

void Primitives::Start(const glm::mat4& viewProjection)
{
	s_renderData->Shader->Use();
	s_renderData->Shader->SetUniformMat4("u_ViewProjection", viewProjection);
}

void Primitives::Flush()
{
	if (s_renderData->Vertices.empty())
		return;

	s_renderData->VertexArray->Bind();
	s_renderData->VertexArray->GetArrayBuffer(0)->BufferSubData(0, s_renderData->Vertices.size() * sizeof(glm::vec2), &s_renderData->Vertices[0]);

	s_renderData->Shader->Use();

	glDrawArrays(GL_LINES, 0, s_renderData->Vertices.size());

	s_renderData->Vertices.clear();
}

void Primitives::Vertex(const glm::vec2& v)
{
	if (s_renderData->Vertices.size() >= MAX_VERTICES)
		Flush();

	s_renderData->Vertices.push_back(v);
}

void Primitives::DrawLine(const glm::vec2& start, const glm::vec2& end)
{
	Vertex(start);
	Vertex(end);
}

void Primitives::DrawRect(float x, float y, float w, float h)
{
	std::vector<glm::vec2> vertices =
	{
		glm::vec2(x, y),
		glm::vec2(x + w, y),
		glm::vec2(x + w, y + h),
		glm::vec2(x, y + h)
	};

	DrawPolygon(vertices);
}

void Primitives::DrawRect(const glm::vec2& pos, const glm::vec2& dim)
{
	DrawRect(pos.x, pos.y, dim.x, dim.y);
}

void Primitives::DrawPolygon(const std::vector<glm::vec2>& vertices)
{
	glm::vec2 p1 = vertices.back();

	for (auto& p2 : vertices)
	{
		Vertex(p1);
		Vertex(p2);
		p1 = p2;
	}
}
