#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ignis
{
	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> VertexArray;
		std::shared_ptr<Shader> Shader;
	};

	static Renderer2DStorage* RenderData;

	void Renderer2D::Init()
	{
		RenderData = new Renderer2DStorage();

		RenderData->VertexArray = std::make_shared<VertexArray>();

		float vertices[3 * 4] = 
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		auto vbo = RenderData->VertexArray->AddArrayBuffer(sizeof(vertices), vertices, GL_STATIC_DRAW);
		vbo->VertexAttribPointer(0, 3, 3 * sizeof(float), 0);

		RenderData->VertexArray->LoadElementBuffer({ 0, 1, 2, 2, 3, 0 }, GL_STATIC_DRAW);

		RenderData->Shader = std::make_shared<Shader>("res/shaders/color.vert", "res/shaders/color.frag");
	}

	void Renderer2D::Destroy()
	{
		delete RenderData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		RenderData->Shader->Use();
		RenderData->Shader->SetUniformMat4("u_ViewProjection", camera.GetViewProjection());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::RenderQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		RenderData->Shader->Use();
		RenderData->Shader->SetUniform4f("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size, 1.0f });
		RenderData->Shader->SetUniformMat4("u_Transform", transform);

		RenderData->VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, RenderData->VertexArray->GetElementBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::RenderQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& color)
	{
	}
}
