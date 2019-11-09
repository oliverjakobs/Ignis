#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ignis
{
	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> VertexArray;
		std::shared_ptr<Texture> Blank;
		std::shared_ptr<Shader> Shader;
	};

	static Renderer2DStorage* s_renderData;

	void Renderer2D::Init()
	{
		s_renderData = new Renderer2DStorage();

		s_renderData->VertexArray = std::make_shared<VertexArray>();

		float vertices[5 * 4] = 
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		s_renderData->VertexArray->AddArrayBuffer(std::make_shared<ArrayBuffer>(sizeof(vertices), vertices, GL_STATIC_DRAW),
			{
				{GL_FLOAT, 3},
				{GL_FLOAT, 2}
			});

		s_renderData->VertexArray->LoadElementBuffer({ 0, 1, 2, 2, 3, 0 }, GL_STATIC_DRAW);

		GLuint blankTextureData = 0xffffffff;
		s_renderData->Blank = std::make_shared<Texture>(1, 1, &blankTextureData);

		s_renderData->Shader = std::make_shared<Shader>("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
		s_renderData->Shader->Use();
		s_renderData->Shader->SetUniform1i("u_Texture", 0);
	}

	void Renderer2D::Destroy()
	{
		delete s_renderData;
	}

	void Renderer2D::BeginScene(const glm::mat4& viewProjection)
	{
		s_renderData->Shader->Use();
		s_renderData->Shader->SetUniformMat4("u_ViewProjection", viewProjection);
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::RenderQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size, 1.0f });

		s_renderData->Shader->SetUniform4f("u_Color", color);
		s_renderData->Shader->SetUniformMat4("u_Model", model);

		s_renderData->Blank->Bind();

		s_renderData->VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, s_renderData->VertexArray->GetElementCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::RenderQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size, 1.0f });

		s_renderData->Shader->SetUniform4f("u_Color", glm::vec4(1.0f));
		s_renderData->Shader->SetUniformMat4("u_Model", model);

		texture->Bind();

		s_renderData->VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, s_renderData->VertexArray->GetElementCount(), GL_UNSIGNED_INT, nullptr);
	}
}
