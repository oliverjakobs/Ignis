#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ignis
{
	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> VertexArray;
		std::shared_ptr<Shader> ColorShader;
		std::shared_ptr<Shader> TextureShader;
	};

	static Renderer2DStorage* RenderData;

	void Renderer2D::Init()
	{
		RenderData = new Renderer2DStorage();

		RenderData->VertexArray = std::make_shared<VertexArray>();

		float vertices[5 * 4] = 
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		
		RenderData->VertexArray->AddArrayBuffer(std::make_shared<ArrayBuffer>(sizeof(vertices), vertices, GL_STATIC_DRAW), 
			{
				{GL_FLOAT, 3},
				{GL_FLOAT, 2}
			});

		RenderData->VertexArray->LoadElementBuffer({ 0, 1, 2, 2, 3, 0 }, GL_STATIC_DRAW);

		RenderData->ColorShader = std::make_shared<Shader>("res/shaders/renderer2D/color.vert", "res/shaders/renderer2D/color.frag");
		RenderData->TextureShader = std::make_shared<Shader>("res/shaders/renderer2D/texture.vert", "res/shaders/renderer2D/texture.frag");
		RenderData->TextureShader->Use();
		RenderData->TextureShader->SetUniform1i("u_Texture", 0);
	}

	void Renderer2D::Destroy()
	{
		delete RenderData;
	}

	void Renderer2D::BeginScene(const glm::mat4& viewProjection)
	{
		RenderData->ColorShader->Use();
		RenderData->ColorShader->SetUniformMat4("u_ViewProjection", viewProjection);

		RenderData->TextureShader->Use();
		RenderData->TextureShader->SetUniformMat4("u_ViewProjection", viewProjection);
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::RenderQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size, 1.0f });

		RenderData->ColorShader->Use();
		RenderData->ColorShader->SetUniform4f("u_Color", color);
		RenderData->ColorShader->SetUniformMat4("u_Model", model);

		RenderData->VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, RenderData->VertexArray->GetElementBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::RenderQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size, 1.0f });

		RenderData->TextureShader->Use();
		RenderData->TextureShader->SetUniformMat4("u_Model", model);

		texture->Bind();

		RenderData->VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, RenderData->VertexArray->GetElementBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
